# Generated automatically from Makefile.in by configure.
srcdir = .
top_srcdir = ../..
top_builddir = ../..
my_dir = lib/ext2fs
INSTALL = /usr/bin/install -c


# Beginning of file MCONFIG

all::

check::

SHELL = /bin/sh

prefix = /usr
root_prefix = 
exec_prefix = ${prefix}
root_bindir = $(root_prefix)/bin
root_sbindir = $(root_prefix)/sbin
root_libdir = $(root_prefix)/lib
bindir = ${exec_prefix}/bin
sbindir = ${exec_prefix}/sbin
libdir = ${exec_prefix}/lib
includedir = ${prefix}/include
mandir = ${prefix}/man
man1dir = $(mandir)/man1
man3dir = $(mandir)/man3
man8dir = $(mandir)/man8
infodir = ${prefix}/info
datadir = ${prefix}/share



INSTALL_PROGRAM = ${INSTALL}
INSTALL_DATA = ${INSTALL} -m 644
CC = cc
BUILD_CC = cc
DEFS =  -DENABLE_SWAPFS=1 -DPACKAGE=\"e2fsprogs\" -DVERSION=\"1.25\" -DSTDC_HEADERS=1 -DHAVE_ALLOCA_H=1 -DHAVE_ALLOCA=1 -DHAVE_UNISTD_H=1 -DHAVE_GETPAGESIZE=1 -DHAVE_MMAP=1 -DHAVE_ARGZ_H=1 -DHAVE_LIMITS_H=1 -DHAVE_LOCALE_H=1 -DHAVE_NL_TYPES_H=1 -DHAVE_MALLOC_H=1 -DHAVE_STRING_H=1 -DHAVE_UNISTD_H=1 -DHAVE_SYS_PARAM_H=1 -DHAVE_GETCWD=1 -DHAVE_MUNMAP=1 -DHAVE_PUTENV=1 -DHAVE_SETENV=1 -DHAVE_SETLOCALE=1 -DHAVE_STRCHR=1 -DHAVE_STRCASECMP=1 -DHAVE_STRDUP=1 -DHAVE___ARGZ_COUNT=1 -DHAVE___ARGZ_STRINGIFY=1 -DHAVE___ARGZ_NEXT=1 -DHAVE_STPCPY=1 -DHAVE_STPCPY=1 -DHAVE_LC_MESSAGES=1 -DHAVE_STDLIB_H=1 -DHAVE_UNISTD_H=1 -DHAVE_STDARG_H=1 -DHAVE_ERRNO_H=1 -DHAVE_MALLOC_H=1 -DHAVE_MNTENT_H=1 -DHAVE_PATHS_H=1 -DHAVE_DIRENT_H=1 -DHAVE_GETOPT_H=1 -DHAVE_SETJMP_H=1 -DHAVE_SIGNAL_H=1 -DHAVE_TERMIOS_H=1 -DHAVE_LINUX_FD_H=1 -DHAVE_LINUX_MAJOR_H=1 -DHAVE_SYS_IOCTL_H=1 -DHAVE_SYS_MOUNT_H=1 -DHAVE_SYS_SYSMACROS_H=1 -DHAVE_SYS_TIME_H=1 -DHAVE_SYS_STAT_H=1 -DHAVE_SYS_TYPES_H=1 -DHAVE_NET_IF_H=1 -DHAVE_NETINET_IN_H=1 -DHAVE_VPRINTF=1 -DHAVE_LSEEK64_PROTOTYPE=1 -DSIZEOF_SHORT=2 -DSIZEOF_INT=4 -DSIZEOF_LONG=4 -DSIZEOF_LONG_LONG=8 -DWORDS_BIGENDIAN=1 -DHAVE_GETRUSAGE=1 -DHAVE_LLSEEK=1 -DHAVE_LSEEK64=1 -DHAVE_OPEN64=1 -DHAVE_STRCASECMP=1 -DHAVE_SRANDOM=1 -DHAVE_FCHOWN=1 -DHAVE_MALLINFO=1 -DHAVE_FDATASYNC=1 -DHAVE_STRNLEN=1 -DHAVE_EXT2_IOCTLS=1 
CFLAGS = -g -O  -G 0 -fno-pic -mno-abicalls
CPPFLAGS = 
ALL_CFLAGS = $(CPPFLAGS) $(DEFS) $(USE_WFLAGS) $(CFLAGS) $(XTRA_CFLAGS) \
	-I. $(LINUX_INCLUDE) 
LDFLAGS = 
ALL_LDFLAGS = $(LDFLAGS)
RM = /bin/rm
LN = /bin/ln
LN_S = ln -s
MV = /bin/mv
CP = /bin/cp
CHMOD = /bin/chmod
AR = ar
AWK = /usr/bin/awk
SED = /bin/sed
PERL = /usr/bin/perl
RANLIB = ranlib
STRIP = strip
LD = $(PURE) cc
ARUPD = $(AR) r
LDCONFIG = :

#
# Library definitions
#
LIB = $(top_builddir)/lib
LIBSS = $(LIB)/libss.a
LIBCOM_ERR = $(LIB)/libcom_err.a
LIBE2P = $(LIB)/libe2p.a
LIBEXT2FS = $(LIB)/libext2fs.a
LIBUUID = $(LIB)/libuuid.a  
DEPLIBUUID = $(LIB)/libuuid.a

STATIC_LIBSS = $(LIB)/libss.a
STATIC_LIBCOM_ERR = $(LIB)/libcom_err.a
STATIC_LIBE2P = $(LIB)/libe2p.a
STATIC_LIBEXT2FS = $(LIB)/libext2fs.a
STATIC_LIBUUID = $(LIB)/libuuid.a  
DEPSTATIC_LIBUUID = $(LIB)/libuuid.a

PROFILED_LIBSS = $(LIB)/libss
PROFILED_LIBCOM_ERR = $(LIB)/libcom_err
PROFILED_LIBE2P = $(LIB)/libe2p
PROFILED_LIBEXT2FS = $(LIB)/libext2fs
PROFILED_LIBUUID = $(LIB)/libuuid  
DEPPROFILED_LIBUUID = $(LIB)/libuuid

#
# Use these definitions if you use tools 2.16 or above
#
DLL_BIN=/usr/bin
JUMP_PREFIX=/usr/bin/jump

# An include directive pointing to a directory holding enough linux-like
# include files to satisfy some programs here
LINUX_INCLUDE=

#
# A fast substitution command for fixing up man pages, shell scripts, etc.
#
SUBST_CONF=util/subst.conf
SUBSTITUTE=util/subst -f $(SUBST_CONF)
DEP_SUBSTITUTE=util/subst $(SUBST_CONF)

$(top_builddir)/util/subst:
	cd $(top_builddir)/util ; $(MAKE) subst

#
# Warning flags
#
# Run make gcc-wall to do a build with warning messages.
#
#
WFLAGS=		-ansi -D_POSIX_SOURCE -pedantic \
			-Wall -Wwrite-strings -Wpointer-arith \
			-Wcast-qual -Wcast-align -Wtraditional \
			-Wstrict-prototypes -Wmissing-prototypes \
			-Wnested-externs -Winline -DNO_INLINE_FUNCS -Wshadow 

gcc-wall-new:
	(make USE_WFLAGS="$(WFLAGS)" > /dev/null) 2>&1 | sed -f $(top_srcdir)/util/gcc-wall-cleanup 

gcc-wall:
	make clean > /dev/null
	make gcc-wall-new

#
# Installation user and groups
#
BINGRP=		bin
BINOWN=		bin
BINMODE=	555
INCGRP=		bin
INCOWN=		bin
INCMODE=	444
LIBOWN=		bin
LIBGRP=		bin
LIBMODE=	444
MANGRP=		bin
MANOWN=		bin
MANMODE=	444

#
# Autoconf magic...
#

DEP_LIB_MAKEFILES = $(top_srcdir)/lib/Makefile.elf-lib  \
	$(top_srcdir)/lib/Makefile.dll-lib $(top_srcdir)/lib/Makefile.bsd-lib \
	$(top_srcdir)/lib/Makefile.checker $(top_srcdir)/lib/Makefile.profile

$(top_builddir)/config.status: $(top_srcdir)/configure
	cd $(top_builddir); ./config.status --recheck

$(top_builddir)/lib/substitute_sh: $(top_srcdir)/lib/substitute_sh.in \
		$(top_builddir)/config.status
	cd $(top_builddir); CONFIG_FILES=lib/substitute_sh ./config.status

$(top_builddir)/util/subst.conf: $(top_srcdir)/util/subst.conf.in \
		$(top_builddir)/config.status
	cd $(top_builddir); CONFIG_FILES=util/subst.conf ./config.status

$(top_srcdir)/configure: $(top_srcdir)/configure.in
	cd $(top_srcdir) && autoconf

#
# Make depend magic...
#

.depend: Makefile $(SRCS) $(top_srcdir)/depfix.sed $(top_srcdir)/wordwrap.pl
	if test -n "$(SRCS)" ; then \
		$(CC) -M $(ALL_CFLAGS) $(SRCS) | \
			$(SED) -f $(top_srcdir)/depfix.sed \
			    -e 's; $(srcdir)/; $$(srcdir)/;g' \
			    -e 's; $(top_srcdir)/; $$(top_srcdir)/;g' \
			    -e 's; $(top_builddir)/; $$(top_builddir)/;g' \
			    -e 's; \./; ;g' \
			    -e '/^ *\\$$/d' | \
			$(PERL) $(top_srcdir)/wordwrap.pl > .depend; \
	else :; fi

depend:: .depend
	if test -n "$(SRCS)" ; then \
		sed -e '/^# +++ Dependency line eater +++/,$$d' \
			< $(srcdir)/Makefile.in | cat - .depend \
			> $(srcdir)/Makefile.in.new; \
	if cmp -s $(srcdir)/Makefile.in $(srcdir)/Makefile.in.new ; then \
		$(RM) $(srcdir)/Makefile.in.new ; \
	else \
		$(MV) $(srcdir)/Makefile.in $(srcdir)/Makefile.in.old; \
		$(MV) $(srcdir)/Makefile.in.new $(srcdir)/Makefile.in; \
	fi ; else :; fi

# End of file MCONFIG

DEBUGFS_LIB_OBJS = bb_compat.o cmp_bitmaps.o dupfs.o fileio.o \
	namei.o test_io.o write_bb_file.o 

RESIZE_LIB_OBJS = rs_bitmap.o

E2IMAGE_LIB_OBJS = imager.o

OBJS= $(DEBUGFS_LIB_OBJS) $(RESIZE_LIB_OBJS) $(E2IMAGE_LIB_OBJS) \
	ext2_err.o \
	alloc.o \
	alloc_tables.o \
	badblocks.o \
	bb_inode.o \
	bitmaps.o \
	bitops.o \
	block.o \
	bmap.o \
	check_desc.o \
	closefs.o \
	dblist.o \
	dblist_dir.o \
	dirblock.o \
	dir_iterate.o \
	expanddir.o \
	ext_attr.o \
	finddev.o \
	flushb.o \
	freefs.o \
	gen_bitmap.o \
	get_pathname.o \
	getsize.o \
	icount.o \
	initialize.o \
	inline.o \
	inode.o \
	ismounted.o \
	link.o \
	llseek.o \
	lookup.o \
	mkdir.o \
	mkjournal.o \
	native.o \
	newdir.o \
	openfs.o \
	read_bb.o \
	read_bb_file.o \
	rw_bitmaps.o \
	swapfs.o \
	unix_io.o \
	unlink.o \
	valid_blk.o \
	version.o

SRCS= ext2_err.c \
	$(srcdir)/alloc.c \
	$(srcdir)/alloc_tables.c \
	$(srcdir)/badblocks.c \
	$(srcdir)/bb_compat.c \
	$(srcdir)/bb_inode.c \
	$(srcdir)/bitmaps.c \
	$(srcdir)/bitops.c \
	$(srcdir)/block.c \
	$(srcdir)/bmap.c \
	$(srcdir)/check_desc.c \
	$(srcdir)/closefs.c \
	$(srcdir)/cmp_bitmaps.c \
	$(srcdir)/dblist.c \
	$(srcdir)/dblist_dir.c \
	$(srcdir)/dirblock.c \
	$(srcdir)/dir_iterate.c \
	$(srcdir)/dupfs.c \
	$(srcdir)/expanddir.c \
	$(srcdir)/ext_attr.c \
	$(srcdir)/fileio.c \
	$(srcdir)/finddev.c \
	$(srcdir)/flushb.c \
	$(srcdir)/freefs.c \
	$(srcdir)/gen_bitmap.c \
	$(srcdir)/get_pathname.c \
	$(srcdir)/getsize.c \
	$(srcdir)/icount.c \
	$(srcdir)/imager.c \
	$(srcdir)/initialize.c \
	$(srcdir)/inline.c \
	$(srcdir)/inode.c \
	$(srcdir)/ismounted.c \
	$(srcdir)/link.c \
	$(srcdir)/llseek.c \
	$(srcdir)/lookup.c \
	$(srcdir)/mkdir.c \
	$(srcdir)/mkjournal.c \
	$(srcdir)/namei.c \
	$(srcdir)/native.c \
	$(srcdir)/newdir.c \
	$(srcdir)/openfs.c \
	$(srcdir)/read_bb.c \
	$(srcdir)/read_bb_file.c \
	$(srcdir)/rs_bitmap.c \
	$(srcdir)/rw_bitmaps.c \
	$(srcdir)/swapfs.c \
	$(srcdir)/test_io.c \
	$(srcdir)/unix_io.c \
	$(srcdir)/unlink.c \
	$(srcdir)/valid_blk.c \
	$(srcdir)/version.c \
	$(srcdir)/write_bb_file.c \
	$(srcdir)/tst_badblocks.c \
	$(srcdir)/tst_bitops.c \
	$(srcdir)/tst_byteswap.c \
	$(srcdir)/tst_getsize.c \
	$(srcdir)/tst_iscan.c

HFILES= bitops.h ext2fs.h ext2_io.h ext2_fs.h ext2_ext_attr.h
HFILES_IN=  ext2_err.h ext2_types.h

LIBRARY= libext2fs
LIBDIR= ext2fs

DLL_ADDRESS = 0x66900000
DLL_JUMPSIZE = 0x1000
DLL_GOTSIZE  = 0x1000
DLL_VERSION = 1.2
DLL_IMAGE = libe2fs
DLL_STUB = libext2fs
DLL_LIBS = -L../.. -lcom_err
DLL_MYDIR = ext2fs
DLL_INSTALL_DIR = $(root_libdir)

ELF_VERSION = 2.4
ELF_SO_VERSION = 2
ELF_IMAGE = libext2fs
ELF_MYDIR = ext2fs
ELF_INSTALL_DIR = $(root_libdir)
ELF_OTHER_LIBS = -L../.. -lcom_err

BSDLIB_VERSION = 2.1
BSDLIB_IMAGE = libext2fs
BSDLIB_MYDIR = ext2fs
BSDLIB_INSTALL_DIR = $(root_libdir)


all:: subdirs $(LIBRARY).a

install-shlibs::

subdirs::

$(LIBRARY).a: $(OBJS)
	(if test -r $@; then $(RM) -f $@.bak && $(MV) $@ $@.bak; fi)
	$(ARUPD) $@ $(OBJS)
	-$(RANLIB) $@

.c.o:
	$(CC) $(ALL_CFLAGS) -c $< -o $@
#	$(CC) $(ALL_CFLAGS) -g -pg -o profiled/$*.o -c $<
#	$(CC) $(ALL_CFLAGS) -checker -g -o checker/$*.o -c $<
#	(export JUMP_DIR=`pwd`/jump; $(CC) -B$(JUMP_PREFIX) $(ALL_CFLAGS) \
#		-o jump/$*.o -c $<)
#	$(CC) $(ALL_CFLAGS) -fPIC -o elfshared/$*.o -c $<
#	$(CC) $(ALL_CFLAGS) -fpic -o pic/$*.o -c $<

COMPILE_ET=et/compile_et

DISTFILES= Makefile *.c *.h image

ext2_err.et: $(DEP_SUBSTITUTE) $(srcdir)/ext2_err.et.in
	$(SUBSTITUTE) $(srcdir)/ext2_err.et.in ext2_err.et

ext2_err.c ext2_err.h: ext2_err.et
	$(COMPILE_ET) ext2_err.et

tst_badblocks: tst_badblocks.o freefs.o \
		read_bb_file.o write_bb_file.o badblocks.o 
	$(CC) -o tst_badblocks tst_badblocks.o freefs.o \
		read_bb_file.o write_bb_file.o badblocks.o $(LIBCOM_ERR)

tst_iscan: tst_iscan.o inode.o badblocks.o test_io.o $(STATIC_LIBEXT2FS)
	$(CC) -o tst_iscan tst_iscan.o inode.o badblocks.o test_io.o \
		$(STATIC_LIBEXT2FS) $(LIBCOM_ERR)

tst_getsize: tst_getsize.o getsize.o $(STATIC_LIBEXT2FS)
	$(CC) -o tst_getsize tst_getsize.o getsize.o $(STATIC_LIBEXT2FS) \
		$(LIBCOM_ERR)

tst_ismounted: $(srcdir)/ismounted.c $(STATIC_LIBEXT2FS)
	$(CC) -o tst_ismounted $(srcdir)/ismounted.c -DDEBUG $(ALL_CFLAGS) $(LIBCOM_ERR) 

tst_byteswap: tst_byteswap.o bitops.o $(STATIC_LIBEXT2FS)
	$(CC) -o tst_byteswap tst_byteswap.o bitops.o $(STATIC_LIBEXT2FS) \
		$(LIBCOM_ERR)

tst_bitops: tst_bitops.o inline.o $(STATIC_LIBEXT2FS)
	$(CC) -o tst_bitops tst_bitops.o inline.o \
		$(STATIC_LIBEXT2FS) $(LIBCOM_ERR)

mkjournal: mkjournal.c $(STATIC_LIBEXT2FS)
	$(CC) -o mkjournal $(srcdir)/mkjournal.c -DDEBUG $(STATIC_LIBEXT2FS) $(LIBCOM_ERR) $(ALL_CFLAGS)

check:: tst_badblocks tst_iscan tst_byteswap
	./tst_badblocks
	./tst_iscan
	./tst_byteswap

clean::
	$(RM) -f \#* *.s *.o *.a *~ *.bak core profiled/* checker/* \
		tst_badblocks tst_iscan ext2_err.et ext2_err.c ext2_err.h \
		tst_byteswap tst_ismounted mkjournal \
		../libext2fs.a ../libext2fs_p.a ../libext2fs_chk.a
#
# Hack to parallel makes recognize dependencies correctly.
#
$(top_builddir)/lib/ext2fs/ext2_err.h: ext2_err.h

# +++ Dependency line eater +++
# 
# Makefile dependencies follow.  This must be the last section in
# the Makefile.in file
#
ext2_err.o: ext2_err.c
alloc.o: $(srcdir)/alloc.c $(srcdir)/ext2_fs.h \
 ext2_types.h $(srcdir)/ext2fs.h \
 com_err.h $(srcdir)/ext2_io.h \
 ext2_err.h $(srcdir)/bitops.h
alloc_tables.o: $(srcdir)/alloc_tables.c $(srcdir)/ext2_fs.h \
 ext2_types.h $(srcdir)/ext2fs.h \
 com_err.h $(srcdir)/ext2_io.h \
 ext2_err.h $(srcdir)/bitops.h
badblocks.o: $(srcdir)/badblocks.c $(srcdir)/ext2_fs.h \
 ext2_types.h $(srcdir)/ext2fsP.h \
 $(srcdir)/ext2fs.h com_err.h $(srcdir)/ext2_io.h \
 ext2_err.h $(srcdir)/bitops.h
bb_compat.o: $(srcdir)/bb_compat.c $(srcdir)/ext2_fs.h \
 ext2_types.h $(srcdir)/ext2fsP.h \
 $(srcdir)/ext2fs.h com_err.h $(srcdir)/ext2_io.h \
 ext2_err.h $(srcdir)/bitops.h
bb_inode.o: $(srcdir)/bb_inode.c $(srcdir)/ext2_fs.h \
 ext2_types.h $(srcdir)/ext2fs.h \
 com_err.h $(srcdir)/ext2_io.h \
 ext2_err.h $(srcdir)/bitops.h
bitmaps.o: $(srcdir)/bitmaps.c $(srcdir)/ext2_fs.h \
 ext2_types.h $(srcdir)/ext2fs.h \
 com_err.h $(srcdir)/ext2_io.h \
 ext2_err.h $(srcdir)/bitops.h
bitops.o: $(srcdir)/bitops.c $(srcdir)/ext2_fs.h \
 ext2_types.h $(srcdir)/ext2fs.h \
 com_err.h $(srcdir)/ext2_io.h \
 ext2_err.h $(srcdir)/bitops.h
block.o: $(srcdir)/block.c $(srcdir)/ext2_fs.h \
 ext2_types.h $(srcdir)/ext2fs.h \
 com_err.h $(srcdir)/ext2_io.h \
 ext2_err.h $(srcdir)/bitops.h
bmap.o: $(srcdir)/bmap.c $(srcdir)/ext2_fs.h \
 ext2_types.h $(srcdir)/ext2fs.h \
 com_err.h $(srcdir)/ext2_io.h \
 ext2_err.h $(srcdir)/bitops.h
check_desc.o: $(srcdir)/check_desc.c $(srcdir)/ext2_fs.h \
 ext2_types.h $(srcdir)/ext2fs.h \
 com_err.h $(srcdir)/ext2_io.h \
 ext2_err.h $(srcdir)/bitops.h
closefs.o: $(srcdir)/closefs.c $(srcdir)/ext2_fs.h \
 ext2_types.h $(srcdir)/ext2fsP.h \
 $(srcdir)/ext2fs.h com_err.h $(srcdir)/ext2_io.h \
 ext2_err.h $(srcdir)/bitops.h
cmp_bitmaps.o: $(srcdir)/cmp_bitmaps.c $(srcdir)/ext2_fs.h \
 ext2_types.h $(srcdir)/ext2fs.h \
 com_err.h $(srcdir)/ext2_io.h \
 ext2_err.h $(srcdir)/bitops.h
dblist.o: $(srcdir)/dblist.c $(srcdir)/ext2_fs.h \
 ext2_types.h $(srcdir)/ext2fsP.h \
 $(srcdir)/ext2fs.h com_err.h $(srcdir)/ext2_io.h \
 ext2_err.h $(srcdir)/bitops.h
dblist_dir.o: $(srcdir)/dblist_dir.c $(srcdir)/ext2_fs.h \
 ext2_types.h $(srcdir)/ext2fsP.h \
 $(srcdir)/ext2fs.h com_err.h $(srcdir)/ext2_io.h \
 ext2_err.h $(srcdir)/bitops.h
dirblock.o: $(srcdir)/dirblock.c $(srcdir)/ext2_fs.h \
 ext2_types.h $(srcdir)/ext2fs.h \
 com_err.h $(srcdir)/ext2_io.h \
 ext2_err.h $(srcdir)/bitops.h
dir_iterate.o: $(srcdir)/dir_iterate.c $(srcdir)/ext2_fs.h \
 ext2_types.h $(srcdir)/ext2fsP.h \
 $(srcdir)/ext2fs.h com_err.h $(srcdir)/ext2_io.h \
 ext2_err.h $(srcdir)/bitops.h
dupfs.o: $(srcdir)/dupfs.c $(srcdir)/ext2_fs.h \
 ext2_types.h $(srcdir)/ext2fsP.h \
 $(srcdir)/ext2fs.h com_err.h $(srcdir)/ext2_io.h \
 ext2_err.h $(srcdir)/bitops.h
expanddir.o: $(srcdir)/expanddir.c $(srcdir)/ext2_fs.h \
 ext2_types.h $(srcdir)/ext2fs.h \
 com_err.h $(srcdir)/ext2_io.h \
 ext2_err.h $(srcdir)/bitops.h
ext_attr.o: $(srcdir)/ext_attr.c $(srcdir)/ext2_fs.h \
 ext2_types.h $(srcdir)/ext2_ext_attr.h \
 $(srcdir)/ext2fs.h com_err.h $(srcdir)/ext2_io.h \
 ext2_err.h $(srcdir)/bitops.h
fileio.o: $(srcdir)/fileio.c $(srcdir)/ext2_fs.h \
 ext2_types.h $(srcdir)/ext2fs.h \
 com_err.h $(srcdir)/ext2_io.h \
 ext2_err.h $(srcdir)/bitops.h
finddev.o: $(srcdir)/finddev.c $(srcdir)/ext2_fs.h \
 ext2_types.h $(srcdir)/ext2fs.h \
 com_err.h $(srcdir)/ext2_io.h \
 ext2_err.h $(srcdir)/bitops.h
flushb.o: $(srcdir)/flushb.c $(srcdir)/ext2_fs.h \
 ext2_types.h $(srcdir)/ext2fs.h \
 com_err.h $(srcdir)/ext2_io.h \
 ext2_err.h $(srcdir)/bitops.h
freefs.o: $(srcdir)/freefs.c $(srcdir)/ext2_fs.h \
 ext2_types.h $(srcdir)/ext2fsP.h \
 $(srcdir)/ext2fs.h com_err.h $(srcdir)/ext2_io.h \
 ext2_err.h $(srcdir)/bitops.h
gen_bitmap.o: $(srcdir)/gen_bitmap.c $(srcdir)/ext2_fs.h \
 ext2_types.h $(srcdir)/ext2fs.h \
 com_err.h $(srcdir)/ext2_io.h \
 ext2_err.h $(srcdir)/bitops.h
get_pathname.o: $(srcdir)/get_pathname.c $(srcdir)/ext2_fs.h \
 ext2_types.h $(srcdir)/ext2fs.h \
 com_err.h $(srcdir)/ext2_io.h \
 ext2_err.h $(srcdir)/bitops.h
getsize.o: $(srcdir)/getsize.c $(srcdir)/ext2_fs.h \
 ext2_types.h $(srcdir)/ext2fs.h \
 com_err.h $(srcdir)/ext2_io.h \
 ext2_err.h $(srcdir)/bitops.h
icount.o: $(srcdir)/icount.c $(srcdir)/ext2_fs.h \
 ext2_types.h $(srcdir)/ext2fs.h \
 com_err.h $(srcdir)/ext2_io.h \
 ext2_err.h $(srcdir)/bitops.h
imager.o: $(srcdir)/imager.c $(srcdir)/ext2_fs.h \
 ext2_types.h $(srcdir)/ext2fs.h \
 com_err.h $(srcdir)/ext2_io.h \
 ext2_err.h $(srcdir)/bitops.h
initialize.o: $(srcdir)/initialize.c $(srcdir)/ext2_fs.h \
 ext2_types.h $(srcdir)/ext2fs.h \
 com_err.h $(srcdir)/ext2_io.h \
 ext2_err.h $(srcdir)/bitops.h
inline.o: $(srcdir)/inline.c $(srcdir)/ext2_fs.h \
 ext2_types.h $(srcdir)/ext2fs.h \
 com_err.h $(srcdir)/ext2_io.h \
 ext2_err.h $(srcdir)/bitops.h
inode.o: $(srcdir)/inode.c $(srcdir)/ext2_fs.h \
 ext2_types.h $(srcdir)/ext2fsP.h \
 $(srcdir)/ext2fs.h com_err.h $(srcdir)/ext2_io.h \
 ext2_err.h $(srcdir)/bitops.h $(srcdir)/e2image.h
ismounted.o: $(srcdir)/ismounted.c $(srcdir)/ext2_fs.h \
 ext2_types.h $(srcdir)/ext2fs.h \
 com_err.h $(srcdir)/ext2_io.h \
 ext2_err.h $(srcdir)/bitops.h
link.o: $(srcdir)/link.c $(srcdir)/ext2_fs.h \
 ext2_types.h $(srcdir)/ext2fs.h \
 com_err.h $(srcdir)/ext2_io.h \
 ext2_err.h $(srcdir)/bitops.h
llseek.o: $(srcdir)/llseek.c com_err.h \
 $(srcdir)/ext2_io.h
lookup.o: $(srcdir)/lookup.c $(srcdir)/ext2_fs.h \
 ext2_types.h $(srcdir)/ext2fs.h \
 com_err.h $(srcdir)/ext2_io.h \
 ext2_err.h $(srcdir)/bitops.h
mkdir.o: $(srcdir)/mkdir.c $(srcdir)/ext2_fs.h \
 ext2_types.h $(srcdir)/ext2fs.h \
 com_err.h $(srcdir)/ext2_io.h \
 ext2_err.h $(srcdir)/bitops.h
mkjournal.o: $(srcdir)/mkjournal.c $(srcdir)/ext2_fs.h \
 ext2_types.h e2p/e2p.h \
 $(srcdir)/ext2fs.h com_err.h $(srcdir)/ext2_io.h \
 ext2_err.h $(srcdir)/bitops.h \
 $(srcdir)/jfs_user.h linux/jfs.h \
 linux/jfs_compat.h linux/list.h
namei.o: $(srcdir)/namei.c $(srcdir)/ext2_fs.h \
 ext2_types.h $(srcdir)/ext2fs.h \
 com_err.h $(srcdir)/ext2_io.h \
 ext2_err.h $(srcdir)/bitops.h
native.o: $(srcdir)/native.c $(srcdir)/ext2_fs.h \
 ext2_types.h $(srcdir)/ext2fs.h \
 com_err.h $(srcdir)/ext2_io.h \
 ext2_err.h $(srcdir)/bitops.h
newdir.o: $(srcdir)/newdir.c $(srcdir)/ext2_fs.h \
 ext2_types.h $(srcdir)/ext2fs.h \
 com_err.h $(srcdir)/ext2_io.h \
 ext2_err.h $(srcdir)/bitops.h
openfs.o: $(srcdir)/openfs.c $(srcdir)/ext2_fs.h \
 ext2_types.h $(srcdir)/ext2fs.h \
 com_err.h $(srcdir)/ext2_io.h \
 ext2_err.h $(srcdir)/bitops.h $(srcdir)/e2image.h
read_bb.o: $(srcdir)/read_bb.c $(srcdir)/ext2_fs.h \
 ext2_types.h $(srcdir)/ext2fs.h \
 com_err.h $(srcdir)/ext2_io.h \
 ext2_err.h $(srcdir)/bitops.h
read_bb_file.o: $(srcdir)/read_bb_file.c $(srcdir)/ext2_fs.h \
 ext2_types.h $(srcdir)/ext2fs.h \
 com_err.h $(srcdir)/ext2_io.h \
 ext2_err.h $(srcdir)/bitops.h
rs_bitmap.o: $(srcdir)/rs_bitmap.c $(srcdir)/ext2_fs.h \
 ext2_types.h $(srcdir)/ext2fs.h \
 com_err.h $(srcdir)/ext2_io.h \
 ext2_err.h $(srcdir)/bitops.h
rw_bitmaps.o: $(srcdir)/rw_bitmaps.c $(srcdir)/ext2_fs.h \
 ext2_types.h $(srcdir)/ext2fs.h \
 com_err.h $(srcdir)/ext2_io.h \
 ext2_err.h $(srcdir)/bitops.h $(srcdir)/e2image.h
swapfs.o: $(srcdir)/swapfs.c $(srcdir)/ext2_fs.h \
 ext2_types.h $(srcdir)/ext2fs.h \
 com_err.h $(srcdir)/ext2_io.h \
 ext2_err.h $(srcdir)/bitops.h
test_io.o: $(srcdir)/test_io.c $(srcdir)/ext2_fs.h \
 ext2_types.h $(srcdir)/ext2fs.h \
 com_err.h $(srcdir)/ext2_io.h \
 ext2_err.h $(srcdir)/bitops.h
unix_io.o: $(srcdir)/unix_io.c $(srcdir)/ext2_fs.h \
 ext2_types.h $(srcdir)/ext2fs.h \
 com_err.h $(srcdir)/ext2_io.h \
 ext2_err.h $(srcdir)/bitops.h
unlink.o: $(srcdir)/unlink.c $(srcdir)/ext2_fs.h \
 ext2_types.h $(srcdir)/ext2fs.h \
 com_err.h $(srcdir)/ext2_io.h \
 ext2_err.h $(srcdir)/bitops.h
valid_blk.o: $(srcdir)/valid_blk.c $(srcdir)/ext2_fs.h \
 ext2_types.h $(srcdir)/ext2fs.h \
 com_err.h $(srcdir)/ext2_io.h \
 ext2_err.h $(srcdir)/bitops.h
version.o: $(srcdir)/version.c $(srcdir)/ext2_fs.h \
 ext2_types.h $(srcdir)/ext2fs.h \
 com_err.h $(srcdir)/ext2_io.h \
 ext2_err.h $(srcdir)/bitops.h \
 version.h
write_bb_file.o: $(srcdir)/write_bb_file.c $(srcdir)/ext2_fs.h \
 ext2_types.h $(srcdir)/ext2fs.h \
 com_err.h $(srcdir)/ext2_io.h \
 ext2_err.h $(srcdir)/bitops.h
tst_badblocks.o: $(srcdir)/tst_badblocks.c $(srcdir)/ext2_fs.h \
 ext2_types.h $(srcdir)/ext2fs.h \
 com_err.h $(srcdir)/ext2_io.h \
 ext2_err.h $(srcdir)/bitops.h
tst_bitops.o: $(srcdir)/tst_bitops.c $(srcdir)/ext2_fs.h \
 ext2_types.h $(srcdir)/ext2fs.h \
 com_err.h $(srcdir)/ext2_io.h \
 ext2_err.h $(srcdir)/bitops.h
tst_byteswap.o: $(srcdir)/tst_byteswap.c $(srcdir)/ext2_fs.h \
 ext2_types.h $(srcdir)/ext2fs.h \
 com_err.h $(srcdir)/ext2_io.h \
 ext2_err.h $(srcdir)/bitops.h
tst_getsize.o: $(srcdir)/tst_getsize.c $(srcdir)/ext2_fs.h \
 ext2_types.h $(srcdir)/ext2fs.h \
 com_err.h $(srcdir)/ext2_io.h \
 ext2_err.h $(srcdir)/bitops.h
tst_iscan.o: $(srcdir)/tst_iscan.c $(srcdir)/ext2_fs.h \
 $(top_builddir)/lib/ext2fs/ext2_types.h $(srcdir)/ext2fs.h \
 com_err.h $(srcdir)/ext2_io.h \
 $(top_builddir)/lib/ext2fs/ext2_err.h $(srcdir)/bitops.h
