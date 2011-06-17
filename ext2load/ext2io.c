/*
 * extio.c
 *
 * Copyright 1999 Silicon Graphics, Inc.
 *           2001-2004 Guido Guenther <agx@sigxcpu.org>
 *
 * Derived from e2fsprogs lib/ext2fs/unix_io.c
 * Copyright (C) 1993, 1994, 1995 Theodore Ts'o.
 */
/* #define ARC_IO_ALLOW_WRITE */

#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <ext2_fs.h>
#include <ext2fs.h>
#include <arc.h>

/*
 * All About the Cache
 *
 * Without this cache, reading is horribly slow - it can take 30-60 seconds
 * (or even more) to read a kernel.  While this is a bootloader and we only
 * do it once, that's still a very long time for the user to sit there with
 * nothing happening (a progress indicator has also been added).  The
 * read workload looks like this: reads of the inode and indirection blocks
 * interleaved with single-block reads of what are essentially contiguous
 * regions of data blocks.  As a concrete example, we might see:
 *
 * 100, 200, 100, 201, 100, 202, 100, 101, 203, 100, 101, 204, ...
 *
 * Therefore we could simply cache the last 4 or so blocks and get an
 * immediate 50-67% speedup with minimal waste.  However, it's possible to
 * do better - in fact, a lot better.  The ARCS calls are so expensive that
 * it's worthwhile also to try doing readahead.  Unless the filesystem is
 * horribly fragmented, this will up our hit ratio to at least 85% or so
 * with just 16 cache blocks (in fact if the fs is 0% fragmented, we could
 * see 99% hits on the indirection blocks and about 92% on the data blocks,
 * or about 96% overall! - even 80% would be adequate however).
 *
 * We really have two caches: a traditional LRU single-block cache, and a
 * readahead multiblock scatter/gather cache.  They are however unified to
 * speed lookup.  CACHE_SIZE is the total number of cacheable blocks, and
 * CACHE_SG_MAX is the maximum size of a s/g request.  The overall
 * implementation is based on the one in unix_io, but has a lot of changes
 * to accomodate readahead.
 *
 * Lookup is straightforward: the cache is fully associative, so we do a
 * linear search for the requested block number (it is only possible to
 * search for one block at a time).  Alloc requests are handled differently.
 * We start with the age of the oldest block and work newer until we have
 * enough blocks to satisfy the sg request.  These blocks have their bufs
 * point into the per-cache arc_sg_buf and the number of successfully allocated
 * blocks is then returned after invalidating each allocated cache block and
 * recording the block it will reference.  A later call to fill_sg_blocks
 * will perform a single read to fill the entire cache "line."
 *
 * When any sg cache block is reused, the sg cached data is first copied into
 * the per-cache buffer for all sg cache blocks, and then all buffer pointers
 * in the sg cache blocks are reset.  Note that we only do this for the
 * cache blocks we aren't going to immediately reuse.
 *
 * We don't have any reliable replacement for time(2), so instead we just use
 * a monotonically increasing counter incremented by any function that looks
 * into the cache.  We do risk overflow, but if we do 2**32 cache lookups
 * the machine has probably failed to do anything useful anyway.
 *
 * Important: there are two large simplifying assumptions here:
 * (1) The filesystem is universally read-only.  There are no other processes
 *     which can write to this filesystem on this or any remote system.
 * (2) We are single-threaded.
 *
 * As such, we do not have code here to handle locking, coherency, or aliases.
 * This is fine for a bootloader but dangerous in other situations.  If
 * ARC_IO_ALLOW_WRITE is enabled (it's off by default), then on any write will
 * the cache will act as write-through, and the entire cache will be
 * invalidated.  This is the most naive correct implementation.  If writing
 * becomes an important task, this will need to be revisited; the unix_io
 * writeback cache is a good starting point.
 */

#define CACHE_SIZE	16
#define CACHE_SG_MAX	12

#define CACHE_IS_SG(_cache)	((_cache)->buf != (_cache)->alloc_buf)

struct arc_cache {
	char		*buf;
	char		*alloc_buf;
	unsigned long	block;
	int		last_use;
	int		in_use:1;
};

static struct arc_cache *sg_cblocks[CACHE_SG_MAX];
static unsigned long virtual_time;

struct arc_private_data {
	int			magic;
	OPENMODE		mode;
	ULONG			fileID;
	struct arc_cache	cache[CACHE_SIZE];
	char			*arc_sg_buf;
	unsigned long		total_read;
	unsigned long		seek_pos;
	int			seek_pos_valid:1;
};

static void arc_progress(struct arc_private_data *, unsigned long);

static errcode_t alloc_cache(io_channel, struct arc_private_data *);
static void free_cache(struct arc_private_data *);
static void reset_one_cache(io_channel, struct arc_cache *, int);
static void reset_sg_cache(io_channel, struct arc_private_data *, int);
static struct arc_cache *find_cached_block( struct arc_private_data *, 
    unsigned long);
static int alloc_sg_blocks(io_channel, struct arc_private_data *,
    unsigned long, int);
static errcode_t fill_sg_blocks(io_channel, struct arc_private_data *, int);

static errcode_t raw_read_blk(io_channel, struct arc_private_data *,
    unsigned long, int, char *);
static void mul64(unsigned long, int, LARGEINTEGER *);
static errcode_t arc_seek(io_channel, unsigned long);

static errcode_t arc_open(const char *name, int flags, io_channel * channel);
static errcode_t arc_close(io_channel channel);
static errcode_t arc_set_blksize(io_channel channel, int blksize);
static errcode_t arc_read_blk
    (io_channel channel, unsigned long block, int count, void *data);
static errcode_t arc_write_blk
    (io_channel channel, unsigned long block, int count, const void *data);
static errcode_t arc_flush(io_channel channel);

static struct struct_io_manager struct_arc_manager = {
	.magic = EXT2_ET_MAGIC_IO_MANAGER,
	.name = "ARC PROM I/O Manager",
	.open = arc_open,
	.close = arc_close,
	.set_blksize = arc_set_blksize,
	.read_blk = arc_read_blk,
	.write_blk = arc_write_blk,
	.flush = arc_flush,
};
io_manager arc_io_manager = &struct_arc_manager;

int arc_do_progress = 0;

static int hits, misses;

static void
arc_progress(struct arc_private_data *priv, unsigned long count)
{
	int hitrate_w = (hits * 1000) / (hits + misses) / 10;
	int hitrate_f = (hits * 1000) / (hits + misses) % 10;

	priv->total_read += count;
	printf("\r%lx      (cache: %u.%u%%)", priv->total_read, hitrate_w,
	    hitrate_f);

#ifdef DEBUG
	if ((hits + misses) % 100 == 0)
		printf("hits: %u misses %u\n\r", hits, misses);
#endif
}

/*
 * Allocates memory for a single file's cache.
 */
static errcode_t
alloc_cache(io_channel channel, struct arc_private_data *priv)
{
	errcode_t		status;
	struct arc_cache	*cache;
	int			i;

	for(i = 0, cache = priv->cache; i < CACHE_SIZE; i++, cache++) {
		memset(cache, 0, sizeof (struct arc_cache));
		if ((status = ext2fs_get_mem(channel->block_size,
		    (void **) &cache->alloc_buf)) != 0)
			return (status);
		cache->buf = cache->alloc_buf;
	}

	return (ext2fs_get_mem(channel->block_size * CACHE_SG_MAX,
	    (void **) &priv->arc_sg_buf));
}

/*
 * Frees all memory associated with a single file's cache.
 */
static void
free_cache(struct arc_private_data *priv)
{
	struct arc_cache	*cache;
	int			i;

	for (i = 0, cache = priv->cache; i < CACHE_SIZE; i++, cache++) {
		if (cache->alloc_buf)
			ext2fs_free_mem((void **) &cache->alloc_buf);
		memset(cache, 0, sizeof (struct arc_cache));
	}

	ext2fs_free_mem((void **) &priv->arc_sg_buf);
}

/*
 * Resets a cache block.  If the cache block is a valid sg block, the contents
 * will be copied from the sg buffer into the private buffer.  For all blocks,
 * the private buffer will be current.  If discard is set, the block will
 * also be invalidated.
 */
static void
reset_one_cache(io_channel channel, struct arc_cache *cache, int discard)
{
	if (CACHE_IS_SG(cache) && discard == 0 && cache->in_use != 0)
		memcpy(cache->alloc_buf, cache->buf, channel->block_size);

	if (discard != 0)
		cache->in_use = 0;

	cache->buf = cache->alloc_buf;
}

/*
 * Resets all sg cache blocks.  If a block is in the first
 * alloc_count entries in sg_cblocks (meaning it has been allocated for
 * immediate reuse) then also discards the contents.
 */
static void
reset_sg_cache(io_channel channel, struct arc_private_data *priv,
    int alloc_count)
{
	struct arc_cache	*cache;
	int			i, j, discard;

	for (i = 0, cache = priv->cache; i < CACHE_SIZE; i++, cache++) {
		if (CACHE_IS_SG(cache)) {
			discard = 0;
			for (j = 0; j < alloc_count; j++) {
				if (sg_cblocks[j] == cache) {
					discard = 1;
					break;
				}
			}
			reset_one_cache(channel, cache, discard);
		}
	}
}

/*
 * Read count blocks starting at block directly from channel into buf, which
 * must be of size >= channel->block_size * count.  No attempt is made to
 * use or update any caches; however, if the last ARC read left the file
 * pointer at the requested block, we avoid seeking.
 */
static errcode_t
raw_read_blk(io_channel channel, struct arc_private_data *priv,
    unsigned long block, int count, char *buf)
{
	errcode_t	status;
	size_t		length = 0;

	if (priv->seek_pos_valid == 0 || priv->seek_pos != block) {
		status = arc_seek(channel, block);
		priv->seek_pos = block + count;
	} else {
		status = 0;
	}
	/* If something fails, priv->seek_pos is bogus. */
	priv->seek_pos_valid = 0;
	if (status == 0) {
		length = (count < 0) ? -count : count * channel->block_size;
		ULONG nread = 0;

		status = ArcRead(priv->fileID, buf, length, &nread);
		if ((nread > 0) && (nread < length)) {
			status = EXT2_ET_SHORT_READ;
			memset(((char *) buf) + nread, 0, length - nread);
		}
		if (status != 0 && channel->read_error != NULL) {
			status = (channel->read_error)
			    (channel, block, count, buf, length, nread, status);
		}
	} else {
		status = EXT2_ET_BAD_BLOCK_NUM;
	}

	if (status == 0) {
		priv->seek_pos_valid = 1;
		if (arc_do_progress != 0)
			arc_progress(priv, (unsigned long) length);
	}

	return (status);
}

/*
 * For the file associated with priv, find block in the cache.
 * In the case of a miss, return NULL.  The last access "time" will be
 * updated to refresh the LRU.  Note that this is much different from the
 * unix_io.c version of the same function; because our allocation step is
 * far more complex to cover readahead, it is dealt with in alloc_sg_blocks.
 */
static struct arc_cache *
find_cached_block(struct arc_private_data *priv, unsigned long block)
{
	struct arc_cache	*cache;
	int			i;

	++virtual_time;

	for (i = 0, cache = priv->cache; i < CACHE_SIZE; i++, cache++)
		if (cache->block == block) {
			cache->last_use = virtual_time;
			++hits;
			return (cache);
		}

	++misses;
	return (NULL);
}

/*
 * Allocate a set of cache blocks whose buffers are contiguous.  The cache
 * blocks are found in sg_cblocks.  The number of allocated blocks is the
 * return value; a return value of 0 indicates an error.  The cache blocks
 * are not filled here; use fill_sg_blocks for that.
 */
static int
alloc_sg_blocks(io_channel channel, struct arc_private_data *priv,
    unsigned long block, int count)
{
	struct arc_cache	*cache, *oldest_cache;
	int			i, unused_count, age_mark;

	if (count > CACHE_SG_MAX)
		count = CACHE_SG_MAX;

	++virtual_time;
	oldest_cache = NULL;
	unused_count = 0;

	/* First use unused blocks, if any are available. */
	for (i = 0, cache = priv->cache; i < CACHE_SIZE && unused_count < count;
	    i++, cache++) {
		if (cache->in_use == 0) {
			sg_cblocks[unused_count++] = cache;
			continue;
		}
		if (!oldest_cache || cache->last_use < oldest_cache->last_use)
			oldest_cache = cache;
	}

	/* If we don't have enough blocks yet, evict the LRUs. */
	if (unused_count < count) {
		for (age_mark = oldest_cache->last_use;
		    unused_count < count && (unsigned long)age_mark <= virtual_time;
		    age_mark++) {
			for (i = 0, cache = priv->cache;
			    i < CACHE_SIZE && unused_count < count;
			    i++, cache++) {
				if (cache->in_use == 0)
					continue;
				if (cache->last_use == age_mark)
					sg_cblocks[unused_count++] = cache;
			}
		}
	}

	/*
	 * At this point it's impossible not to have count blocks.  However,
	 * even if we somehow don't, it's not fatal - perhaps someone
	 * decided to use some future lru timestamp to lock an entry or
	 * something.  In this case, we just continue on, and make sure the
	 * caller knows we didn't allocate as much as was requested.
	 */

	/*
	 * Now we set up the cache blocks.  Their buffers need to be
	 * set to the sg buffer and they must be marked invalid (we will
	 * mark them valid once fill_sg_blocks fills them).
	 */
	reset_sg_cache(channel, priv, count);

	for (i = 0; i < count; i++) {
		cache = sg_cblocks[i];
		cache->in_use = 0;
		cache->block = block + i;
		cache->buf = priv->arc_sg_buf + i * channel->block_size;
	}

	return (count);
}

/*
 * Fill the first count cache blocks in sg_cblocks with contiguous data from
 * the file.  The block numbers are already stored in the cache metadata
 * by a mandatory previous call to alloc_sg_blocks.  This can fail if there
 * is an i/o error.
 */
static errcode_t
fill_sg_blocks(io_channel channel, struct arc_private_data *priv, int count)
{
	errcode_t	status;
	int		i;

	status = raw_read_blk(channel, priv, sg_cblocks[0]->block, count,
	    priv->arc_sg_buf);

	/*
	 * XXX Handle short read here: it may be that we've reached EOF and
	 * can mark some of the blocks valid.
	 */
	if (status == 0) {
		for (i = 0; i < count; i++) {
			sg_cblocks[i]->in_use = 1;
			sg_cblocks[i]->last_use = virtual_time;
		}
	}

	return (status);
}

/*
 * Mark the entire contents of the cache invalid, and reset any sg blocks
 * to private buffers.
 */
static void
cache_invalidate(io_channel channel, struct arc_private_data *priv)
{
	struct arc_cache	*cache;
	int			i;

	for (i = 0, cache = priv->cache; i < CACHE_SIZE; i++, cache++)
		reset_one_cache(channel, cache, 1);
}

static errcode_t
arc_open(const char *name, int flags, io_channel * pchannel)
{
	io_channel channel = NULL;
	struct arc_private_data *priv;
	errcode_t status;

	if (name == NULL)
		return EXT2_ET_BAD_DEVICE_NAME;

	status =
	    ext2fs_get_mem(sizeof(struct struct_io_channel),
			   (void **) &channel);

	if (status == 0) {
		memset(channel, 0, sizeof(struct struct_io_channel));

		channel->name = NULL;
		channel->private_data = NULL;
		channel->magic = EXT2_ET_MAGIC_IO_CHANNEL;
		channel->manager = arc_io_manager;
		channel->block_size = 1024;
		channel->read_error = NULL;
		channel->write_error = NULL;
		channel->refcount = 1;

		status =
		    ext2fs_get_mem(strlen(name) + 1,
				   (void **) &channel->name);
		if (status == 0) {
			strcpy(channel->name, name);

			status =
			    ext2fs_get_mem(sizeof(struct arc_private_data),
					   (void **) &priv);
			if (status == 0) {
				memset(priv, 0,
				    sizeof(struct arc_private_data));
				channel->private_data = priv;
				priv->magic = EXT2_ET_BAD_MAGIC;
				priv->mode =
				    (flags & IO_FLAG_RW) ? OpenReadWrite :
				    OpenReadOnly;
				status =
				    ArcOpen((char *) name, priv->mode,
					    &priv->fileID);
				if( status ) {
					status = EXT2_ET_BAD_DEVICE_NAME;
				}
			}
		}
	}

	if (status == 0)
		status = alloc_cache(channel, priv);

	if (status == 0) {
		*pchannel = channel;
	} else if (channel != NULL) {
		if (channel->name != NULL)
			ext2fs_free_mem((void **) &channel->name);
		if (channel->private_data != NULL)
			ext2fs_free_mem((void **) &channel->private_data);
		ext2fs_free_mem((void **) &channel);
	}
	return status;
}


static errcode_t arc_close(io_channel channel)
{
	struct arc_private_data *priv;
	errcode_t status = 0;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	priv = (struct arc_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(priv, EXT2_ET_BAD_MAGIC);

	if (--channel->refcount == 0) {
		status = ArcClose(priv->fileID);
		free_cache(priv);
		if (channel->name != NULL)
			ext2fs_free_mem((void **) &channel->name);
		if (channel->private_data != NULL)
			ext2fs_free_mem((void **) &channel->private_data);
		ext2fs_free_mem((void **) &channel);
	}

	return status;
}


static errcode_t arc_set_blksize(io_channel channel, int blksize)
{
	struct arc_private_data *priv;
	errcode_t		status;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	priv = (struct arc_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(priv, EXT2_ET_BAD_MAGIC);

	if (channel->block_size != blksize) {
		channel->block_size = blksize;
		free_cache(priv);
		if ((status = alloc_cache(channel, priv)) != 0)
			return (status);
	}
	return 0;
}

static void
mul64(unsigned long block, int blocksize, LARGEINTEGER *result)
{
	ULONG m1l = block & 0x0FFFF, m1h = (block >> 16) & 0x0FFFF;
	ULONG m2l = blocksize & 0x0FFFF, m2h = (blocksize >> 16) & 0x0FFFF;
	ULONG i1 = m1l * m2h, i2 = m1h * m2l;

	result->HighPart =
	    (m1h * m2h) + ((i1 >> 16) & 0x0FFFF) + ((i2 >> 16) & 0x0FFFF);
	i1 =
	    (i1 & 0x0FFFF) + (i2 & 0x0FFFF) +
	    (((m1l * m2l) >> 16) & 0x0FFFF);
	result->LowPart = ((i1 & 0x0FFFF) << 16) + ((m1l * m2l) & 0x0FFFF);
	result->HighPart += (i1 >> 16) & 0x0FFFF;
}

static errcode_t
arc_seek(io_channel channel, unsigned long block)
{
	struct arc_private_data *priv;
	LARGEINTEGER position;

	priv = (struct arc_private_data *) channel->private_data;
	mul64(block, channel->block_size, &position);
	return ArcSeek(priv->fileID, &position, SeekAbsolute);
}

/*
 * Perform a cacheable read.  First, the cache will be checked for an
 * existing copy of the blocks.  If present, they are copied into buf.
 * Otherwise, we set up and execute a readahead, then copy the results into
 * buf.  The unix_io way is a little nicer; since it doesn't have readahead
 * it knows that buf is always big enough in multicount scenarios and thus
 * dispenses with the extra memcpy.  There is an opportunity to improve this.
 */
static errcode_t
arc_read_blk(io_channel channel, unsigned long block, int count, void *buf)
{
	struct arc_private_data *priv;
	errcode_t		status = 0;
	struct arc_cache	*cache;
	char			*cbuf = (char *) buf;
	int			cb_alloc;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	priv = (struct arc_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(priv, EXT2_ET_BAD_MAGIC);

#ifdef DEBUG
	printf("req %lu id %lu count %u\n\r", block, priv->fileID, count);
#endif

	/* Odd-sized reads can't be cached. */
	if (count < 0)
		status = raw_read_blk(channel, priv, block, count, cbuf);

	while (count > 0) {
		if ((cache = find_cached_block(priv, block)) == NULL)
			break;
#ifdef DEBUG
		printf("Cache hit on block %lu\n\r", block);
#endif
		memcpy(cbuf, cache->buf, channel->block_size);
		count--;
		block++;
		cbuf += channel->block_size;
	}

	/*
	 * Cache miss.  Although it could be that there's just a hole
	 * in the cache, it's far more likely and easier to handle
	 * that we've reached the end of a readahead blockset.  Thus
	 * we just stop looking in the cache for the rest until after
	 * we do a readahead.  We could try to put in some
	 * heuristics here to avoid trashing the cache unnecessarily
	 * for reads we expect are not part of a sequential set.
	 */
	while (count > 0) {
#ifdef DEBUG
		printf("Cache miss on block %lu (readahead %u)\n\r",
		    block, CACHE_SG_MAX);
#endif
		if ((cb_alloc = alloc_sg_blocks(channel, priv, block,
		    CACHE_SG_MAX)) == 0) {
#ifdef DEBUG
			printf("%s\n\r", "Cache error: can't alloc any blocks");
#endif
			/* Cache is broken, so do the raw read. */
			cache_invalidate(channel, priv);
			status = raw_read_blk(channel, priv, block, count,
			    cbuf);
			break;
		}

		if ((status = fill_sg_blocks(channel, priv, cb_alloc)) != 0) {
#ifdef DEBUG
			printf("Cache error (status %lu at block %lu(%u)\n\r",
			    (unsigned long) status, block, count);
#endif
			/* Cache is broken, so do the raw read. */
			cache_invalidate(channel, priv);
			status = raw_read_blk(channel, priv, block, count,
			    cbuf);
			break;
		}

		if (cb_alloc >= count) {
			memcpy(cbuf, priv->arc_sg_buf,
			    count * channel->block_size);
			return (0);
		}

		memcpy(cbuf, priv->arc_sg_buf, cb_alloc * channel->block_size);
		count -= cb_alloc;
		block += cb_alloc;
		cbuf += cb_alloc * channel->block_size;
	}

	return (status);
}

static errcode_t
arc_write_blk (io_channel channel,
               unsigned long block, 
               int count __attribute__((unused)),
               const void *buf __attribute__((unused)))
{
	struct arc_private_data *priv;
	errcode_t status;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	priv = (struct arc_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(priv, EXT2_ET_BAD_MAGIC);

	status = arc_seek(channel, block);
#ifdef ARC_IO_ALLOW_WRITE
	cache_invalidate(channel, priv);
	priv->seek_pos_valid = 0;
	if (status == 0) {
		size_t length =
		    (count < 0) ? -count : count * channel->block_size;
		ULONG nwritten = 0;

		status =
		    ArcWrite(priv->fileID, (void *) buf, length,
			     &nwritten);
		if ((nwritten > 0) && (nwritten < length))
			status = EXT2_ET_SHORT_WRITE;
		if ((status != ESUCCESS) && (channel->write_error != NULL)) {
			status = (channel->write_error)
			    (channel, block, count, buf, length, nwritten, status);
		}
	}
#endif				/* ARC_IO_ALLOW_WRITE */

	return status;
}


static errcode_t arc_flush(io_channel channel)
{
	struct arc_private_data *priv;

	EXT2_CHECK_MAGIC(channel, EXT2_ET_MAGIC_IO_CHANNEL);
	priv = (struct arc_private_data *) channel->private_data;
	EXT2_CHECK_MAGIC(priv, EXT2_ET_BAD_MAGIC);

	return 0;
}


/* Hack in some stuff to make ext2fs library work */
time_t time(time_t *t __attribute__((unused)))
{
	return 0;
}

/* We can get away with those two because libext2fs uses them only in
   fileio.c for file size calculations, and the bootloader needs not
   to read files >2GB (famous last words...).  */
unsigned long long __udivdi3(unsigned long long numerator,
			     unsigned long long denominator)
{
	return ((unsigned int)(numerator)) / ((unsigned int)(denominator));
}

unsigned long long __umoddi3(unsigned long long val, unsigned long long mod)
{
	return ((unsigned int)(val)) % ((unsigned int)(mod));
}

struct et_list {
	struct et_list *next;
	const struct error_table *table;
};
struct et_list *_et_list = NULL;

void com_err(const char *whoami __attribute__((unused)),
             long error,
             const char *format __attribute__((unused)),
             ...)
{
	printf("com_err called with %lu\n", error);
}

const char *ext2fs_strerror(long error)
{
	struct et_list *list = _et_list;

	while (list != NULL) {
		if ((error >= list->table->base)
		    && (error < (list->table->base + list->table->n_msgs))) {
			return list->table->msgs[error -
						 list->table->base];
		}

		list = list->next;
	}
	return NULL;
}

void print_ext2fs_error(long error)
{
	const char* msg;

	msg = ext2fs_strerror(error);
	if(msg)
		printf("ext2fs - %s\n\r", msg);
	else
		printf("ext2fs - unknown error (%lu)\n\r", error);
		
}
