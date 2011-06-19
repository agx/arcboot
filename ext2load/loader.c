/*
 * Copyright 1999, 2001 Silicon Graphics, Inc.
 * Copyright 2001 Ralf Baechle
 *           2001-04 Guido Guenther <agx@sigxcpu.org>
 */
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <types.h>
#include <stdint.h>

#include <arc.h>
#include <elf.h>

#include <sys/types.h>
#include <ext2_fs.h>
#include <ext2fs.h>

#include "arcboot.h"

#include <subarch.h>
#include <debug.h>

#define KSEG0ADDR(addr)	(((addr) & 0x1fffffff) | 0x80000000)

#define ANSI_CLEAR	"\033[2J"
#define CONF_FILE	"/etc/arcboot.conf"

static char argv_rd_start[32];
static char argv_rd_size[32];

unsigned long max_page_size = 0;

CHAR *OSLoadPartition = NULL;
CHAR *OSLoadFilename = NULL;
CHAR *OSLoadOptions = NULL;
static int is64=0;


typedef	union {
	unsigned char e_ident[EI_NIDENT];
	Elf32_Ehdr header32;
	Elf64_Ehdr header64;
} Elf_Ehdr;

static void Wait(const char *prompt)
{
	int ch;

	if (prompt != NULL)
		puts(prompt);

	do {
		ch = getchar();
	} while ((ch != EOF) && (((char) ch) != ' '));
}


void Fatal(const CHAR * message, ...)
{
	va_list ap;

	if (message != NULL) {
		printf("FATAL ERROR:  ");
		va_start(ap, message);
		vprintf(message, ap);
		va_end(ap);
	}

	Wait("\n\r--- Press <spacebar> to enter ARC interactive mode ---");
	ArcEnterInteractiveMode();
}


void InitMalloc(void)
{
	MEMORYDESCRIPTOR *current = NULL;
	ULONG stack = (ULONG) &current;
	debug_printf("stack starts at: 0x%lx\n\r", stack);

	current = ArcGetMemoryDescriptor(current);
	if(! current ) {
		Fatal("Can't find any valid memory descriptors!\n\r");
	}
	while (current != NULL) {
		/*
		 *  The spec says we should have an adjacent FreeContiguous
		 *  memory area that includes our stack.  It would be much
		 *  easier to just look for that and give it to malloc, but
		 *  the Indy only shows FreeMemory areas, no FreeContiguous.
		 *  Oh well.
		 */
		if (current->Type == FreeMemory) {
			ULONG start = KSEG0ADDR(current->BasePage * PAGE_SIZE);
			ULONG end =
			    start + (current->PageCount * PAGE_SIZE);
			debug_printf("Free Memory(%u) segment found at (0x%lx,0x%lx).\n\r",
					current->Type, start, end); 

			/* Leave some space for our stack */
			if ((stack >= start) && (stack < end))
				end =
				    (stack -
				     (STACK_PAGES *
				      PAGE_SIZE)) & ~(PAGE_SIZE - 1);
			/* Don't use memory from reserved region */
			if ((start >= kernel_load[SUBARCH].base )
			    && (start < (kernel_load[SUBARCH].base 
					    + kernel_load[SUBARCH].reserved )))
				start = kernel_load[SUBARCH].base 
					    + kernel_load[SUBARCH].reserved;
			if ((end > kernel_load[SUBARCH].base) 
			     && (end <= (kernel_load[SUBARCH].base 
					    + kernel_load[SUBARCH].reserved ))) 
				end = kernel_load[SUBARCH].base;
			if (end > start) {
				debug_printf("Adding %lu bytes at 0x%lx to the list of available memory\n\r",
						end-start, start);
				arclib_malloc_add(start, end - start);
			}
		}
		current = ArcGetMemoryDescriptor(current);
	}
}

int isEnvVar(const char* arg)
{
	unsigned int i;

	for (i = 0; i < NENTS(env_vars); i++) {
		if(strncmp( env_vars[i], arg, strlen(env_vars[i]) ) ==  0)
			return 1;
	}
	return 0;
}

int ProcessArguments(LONG argc, CHAR * argv[])
{
	LONG arg;
	CHAR *equals;
	size_t len;

	/* save some things we need later */
	for (arg = 1; arg < argc; arg++) {
		equals = strchr(argv[arg], '=');
		if (equals != NULL) {
			len = equals - argv[arg];
			if (strncmp(argv[arg], "OSLoadPartition", len) == 0) 
				OSLoadPartition = equals + 1;
			if (strncmp(argv[arg], "OSLoadFilename", len) == 0)
				OSLoadFilename = equals + 1;
			if (strncmp(argv[arg], "OSLoadOptions", len) == 0) {
				/* Copy options to local memory to avoid overwrite later */
				OSLoadOptions = strdup(equals + 1);
				if (OSLoadOptions == NULL)
					Fatal ("Cannot allocate memory for options string\n\r");
			}
		}
	}
	/* 
	 * in case the user typed "boot a b c" the argv looks like:
	 * scsi(0)..(8)/arcboot a  b  c  OSLoadPartition=.. SystemPartition=..
	 * argv:         `0	`-1`-2`-3`-4                `-5
	 * we're interested in a,b,c so scan the command line and check for
	 * each argument if it is an environment variable. We're using a fixed
	 * list instead of ArcGetEnvironmentVariable since e.g. the prom sets
	 * "OSLoadOptions=auto" on reboot but
	 * EnvironmentVariable("OSLoadOptions") == NULL
	 */
	for( arg = 1; arg < argc; arg++ ) {
		if( isEnvVar(argv[arg])) {
			return (arg-1);
#ifdef DEBUG
		} else {
			printf("%s is not an envVar\n\r", argv[arg]);
#endif
		}
	}
	return 0;
}

int LoadProgramSegments32(ext2_file_t file, Elf_Ehdr * header, void *segments)
{
	int idx;
	int loaded = 0;
	Elf32_Phdr* segment=(Elf32_Phdr*)segments;
	errcode_t status;
	size_t size;

	printf("Loading 32-bit executable\n\r");

	for (idx = 0; idx < header->header32.e_phnum; idx++) {
		if(max_page_size == 0) {
                        max_page_size = segment->p_offset;
                }

		if (segment->p_type == PT_LOAD) {
		    printf
			("Loading program segment %u at 0x%x, offset=0x%x, size = 0x%x\n\r",
		        idx + 1, KSEG0ADDR(segment->p_vaddr),
		        segment->p_offset, segment->p_filesz);

		    status =
			ext2fs_file_lseek(file, segment->p_offset,
				        EXT2_SEEK_SET, NULL);
		    if (status != 0) {
			    print_ext2fs_error(status);
			    Fatal("Cannot seek to program segment\n\r");
		    }

		    arc_do_progress = 1;
		    status = ext2fs_file_read(file,
					  (void *) (KSEG0ADDR(
					            segment->p_vaddr)),
					  segment->p_filesz, NULL);
		    printf("\n\n\r");	/* Clear progress */
		    arc_do_progress = 0;
		    if (status != 0) {
			print_ext2fs_error(status);
			Fatal("Cannot read program segment\n\r");
		    }

		    size = segment->p_memsz - segment->p_filesz;
		    if (size > 0) {
			    printf
				("Zeroing memory at 0x%x, size = 0x%x\n\r",
			        (KSEG0ADDR(segment->p_vaddr +
			        segment->p_filesz)), size);
			    memset((void *)
			       (KSEG0ADDR(segment->
				 p_vaddr + segment->p_filesz)), 0, size);
		    }

		    loaded = 1;
	    }

	    segment =
		(Elf32_Phdr *) (((char *) segment) +
			    header->header32.e_phentsize);
    }

	return loaded;
}

int LoadProgramSegments64(ext2_file_t file, Elf_Ehdr * header, void *segments)
{
	int idx;
	int loaded = 0;
 	Elf64_Phdr* segment=(Elf64_Phdr*)segments;
	errcode_t status;
	unsigned long size;

	is64=1;
	printf("Loading 64-bit executable\n\r");

	for (idx = 0; idx < header->header64.e_phnum; idx++) {
		if(max_page_size == 0) {
                        max_page_size = segment->p_offset;
                }

		if (segment->p_type == PT_LOAD) {
		    printf ("Loading program segment %u at 0x%x, "
			    "offset=0x%lx %lx, size = 0x%lx %lx\n\r",
			    idx + 1,
			    (int)KSEG0ADDR(segment->p_vaddr),
			    (long)(segment->p_offset>>32),
			    (long)(segment->p_offset&0xffffffff),
			    (long)(segment->p_filesz>>32),
			    (long)(segment->p_filesz&0xffffffff));

		    status =
			ext2fs_file_lseek(file, segment->p_offset,
				        EXT2_SEEK_SET, NULL);
		    if (status != 0) {
			    print_ext2fs_error(status);
			    Fatal("Cannot seek to program segment\n\r");
		    }

		    arc_do_progress = 1;
		    status = ext2fs_file_read(file,
					  (void *) (KSEG0ADDR((ULONG)
					            segment->p_vaddr)),
					  segment->p_filesz, NULL);
		    arc_do_progress = 0;
		    if (status != 0) {
			print_ext2fs_error(status);
			Fatal("Cannot read program segment\n\r");
		    }

		    size = (ULONG)segment->p_memsz - (ULONG)segment->p_filesz;
		    if (size > 0) {
			    printf
				("Zeroing memory at 0x%lx, size = 0x%lx\n\r",
			        (KSEG0ADDR((ULONG)segment->p_vaddr +
			        (ULONG)segment->p_filesz)), size);
			    memset((void *)
			       (KSEG0ADDR((ULONG)segment->p_vaddr + 
			        (ULONG)segment->p_filesz)), 0, size);
		    }

		    loaded = 1;
		}

		segment = (Elf64_Phdr *) (((char *) segment)
					  + header->header64.e_phentsize);
	}

	return loaded;
}

void LoadProgramSegments(ext2_file_t file, Elf_Ehdr * header)
{
	Boolean loaded = False;
	void *segments;
	size_t size;
	errcode_t status;

	if (header->e_ident[EI_CLASS] == ELFCLASS32)
		size = (size_t) (header->header32.e_phentsize *
				 header->header32.e_phnum);
	else
		size = (size_t) (header->header64.e_phentsize *
				 header->header64.e_phnum);

	if (size <= 0)
		Fatal("No program segments\n\r");

	segments = malloc(size);
	if (segments == NULL)
		Fatal("Cannot allocate memory for segment headers\n\r");
	else
		printf("Allocated 0x%x bytes for segments\n\r",size);

	if (header->e_ident[EI_CLASS] == ELFCLASS32) {
		status = ext2fs_file_lseek(file,
				(ext2_off_t) header->header32.e_phoff,
				EXT2_SEEK_SET, NULL);
	} else {
		status = ext2fs_file_lseek(file,
				(ext2_off_t) header->header64.e_phoff,
				EXT2_SEEK_SET, NULL);
	}
	if (status != 0) {
		print_ext2fs_error(status);
		Fatal("Cannot seek to program segment headers\n\r");
	}

	status = ext2fs_file_read(file, segments, size, NULL);
	if (status != 0) {
		print_ext2fs_error(status);
		Fatal("Cannot read program segment headers\n\r");
	}

	if(header->e_ident[EI_CLASS] == ELFCLASS32)
		loaded = LoadProgramSegments32(file, header, segments);
	else
		loaded = LoadProgramSegments64(file, header, segments);

	if (!loaded)
		Fatal("No loadable program segments found\n\r");

	free(segments);
}


Elf64_Addr LoadKernelFile(ext2_file_t file)
{
	Elf_Ehdr header;
	Elf64_Addr entry;
	errcode_t status;

	status =
	    ext2fs_file_read(file, (void *) &header, sizeof(header), NULL);
	if (status != 0) {
		print_ext2fs_error(status);
		Fatal("Can't read file header\n\r");
	}

	if (memcmp(&(header.e_ident[EI_MAG0]), ELFMAG, SELFMAG) != 0)
		Fatal("Not an ELF file\n\r");

	if (header.e_ident[EI_CLASS] != ELFCLASS32 &&
	    header.e_ident[EI_CLASS] != ELFCLASS64)
		Fatal("Not a 32-bit or 64-bit file\n\r");

	if (header.e_ident[EI_DATA] != ELFDATA2MSB)
		Fatal("Not a big-endian file\n\r");

	if (header.e_ident[EI_VERSION] != EV_CURRENT)
		Fatal("Wrong ELF version\n\r");

	if (header.e_ident[EI_CLASS]==ELFCLASS32) {
	    if (header.header32.e_type != ET_EXEC)
		Fatal("Not an executable file\n\r");

	    if (header.header32.e_machine != EM_MIPS)
		Fatal("Unsupported machine type\n\r");

	    if (header.header32.e_version != EV_CURRENT)
		Fatal("Wrong ELF version\n\r");

 	    entry = (Elf64_Addr) header.header32.e_entry;
	} else {
	    if (header.header64.e_type != ET_EXEC)
		Fatal("Not an executable file\n\r");

	    if (header.header64.e_machine != EM_MIPS)
		Fatal("Unsupported machine type\n\r");

	    if (header.header64.e_version != EV_CURRENT)
		Fatal("Wrong ELF version\n\r");

 	    entry = header.header64.e_entry;
	}

	LoadProgramSegments(file, &header);

	return entry;
}


Boolean OpenFile(const char *partition, const char *filename, ext2_file_t* file)
{
	extern io_manager arc_io_manager;
	ext2_filsys fs;
	ext2_ino_t file_inode;
	errcode_t status;

	initialize_ext2_error_table();

	status = ext2fs_open(partition, 0, 0, 0, arc_io_manager, &fs);
	if (status != 0) {
		print_ext2fs_error(status);
		return False;
	}

	status = ext2fs_namei_follow
	    (fs, EXT2_ROOT_INO, EXT2_ROOT_INO, filename, &file_inode);
	if (status != 0) {
		print_ext2fs_error(status);
		return False;
	}

	status = ext2fs_file_open(fs, file_inode, 0, file);
	if (status != 0) {
		print_ext2fs_error(status);
		return False;
	}
	return True;
}

Elf64_Addr LoadKernel(const char *partition, const char *filename)
{
	ext2_file_t file;

	if(!OpenFile( partition, filename, &file ))
		Fatal("Can't load kernel!\n\r");
	return LoadKernelFile(file);
}


void LoadInitrd(const char *partition, const char *filename, int *argc, char **argv)
{
	ext2_file_t file;
	ULONG initrd_addr, initrd_sz;
	int status;

	if (!OpenFile(partition, filename, &file))
		Fatal("Can't load initrd!\n\r");

	initrd_sz = ext2fs_file_get_size(file);

	initrd_addr = (ULONG)malloc(initrd_sz + max_page_size);

	if (initrd_addr == 0) {
		Fatal("Cannot allocate memory for initrd\n\r");
	}

	initrd_addr = (initrd_addr + max_page_size) & ~(max_page_size - 1);

	printf("Loading initrd at 0x%lx, %lu bytes...\n\r", initrd_addr, initrd_sz);

	arc_do_progress = 1;
	status = ext2fs_file_read(file, (void*) initrd_addr,
				  initrd_sz, NULL);
	arc_do_progress = 0;
	if (status != 0) {
		print_ext2fs_error(status);
		Fatal("Cannot read initrd\n\r");
	}

	/* Add rd_start=, rd_size= */
	sprintf(argv_rd_start, "rd_start=0x%lx", initrd_addr);
	sprintf(argv_rd_size, "rd_size=0x%lx", initrd_sz);
	argv[*argc]=argv_rd_start;
	(*argc)++;
	argv[*argc]=argv_rd_size;
	(*argc)++;
}


void printCmdLine(int argc, CHAR *argv[])
{
 int i;

 for(i = 0; i < argc; i++ )
	printf("%u: %s\n\r", i, argv[i]);
}

void _start64(LONG argc,
              CHAR * argv[],
	      unsigned long long *addr)
{
	__asm__ __volatile__(
		".set push\n"
		"\t.set mips3\n"
		"\t.set noreorder\n"
		"\t.set noat\n"
		"\tld $1, 0(%0)\n"
		"\tmove $4, %1\n"
		"\tmove $5, %2\n"
		"\tjr $1\n"
		"\t nop\n"
		"\t.set pop": : "r" (addr), "r" (argc), "r" (argv) : "$4", "$5");
}

void _start(LONG argc, CHAR *argv[], CHAR *envp[])
{
	CHAR** nargv;
	CHAR** params;
	int nargc, nopt;

	Elf32_Addr kernel_entry32;
	Elf64_Addr kernel_entry64;

	/* Print identification */
	printf(ANSI_CLEAR "\n\rarcsboot: ARCS Linux ext2fs loader "
					__ARCSBOOT_VERSION__ "\n\n\r");

	InitMalloc();

	nopt = ProcessArguments(argc, argv);

#if DEBUG
 	printf("Command line: \n\r");
	printCmdLine(argc, argv);
#endif
	if (nopt) { /* the user typed s.th. on the commandline */
	    OSLoadFilename = argv[1];
	}

	/* Fall back to "Linux" as default name. */
	if (OSLoadFilename == NULL)
		OSLoadFilename = "Linux";

	if (OSLoadPartition == NULL)
		Fatal("Invalid load partition\n\r");
	debug_printf("OSLoadPartition: %s\n\r", OSLoadPartition);
	debug_printf("OSLoadFilename: %s\n\r", OSLoadFilename);
	/*
	 * XXX: let's play stupid for now: assume /etc/arcboot.conf
	 * is on OSLoadPartition
	 */
	if( !(params = ReadConfFile(&OSLoadPartition, CONF_FILE, OSLoadFilename))) {
		printf("Couldn't find label: %s in %s.\n\r", OSLoadFilename, CONF_FILE);
		printf("Will try to boot %s%s.\n\r", OSLoadPartition, OSLoadFilename);
		nargc = argc;
		nargv = argv;
	} else {
		int i;
		OSLoadFilename = params[2];
		nargv = &params[2]; 		/* nargv[0] is the labels name */
		for( nargc=0; nargv[nargc]; nargc++);	/* count nargv argumnts */
		if(OSLoadOptions != NULL) {	/* append OSLoadOptions if present */
			nargv[nargc] = OSLoadOptions;
			nargc++; 
		}
		/* append command line arguments */
		for(i = 2; i <= nopt; i++) {
			nargv[nargc] = argv[i];
			nargc++;
		}
	}
	printf("Loading %s from %s\n\r",(params) ? params[0] : OSLoadFilename, OSLoadPartition);
	kernel_entry64 = LoadKernel(OSLoadPartition, OSLoadFilename);
	kernel_entry32 = (Elf32_Addr) kernel_entry64;

#if DEBUG
	printf("Command line after config file: \n\r");
	printCmdLine(nargc, nargv);
#endif

	if (params[1]) {
	  printf("Loading initrd %s from %s\n\r", params[1], OSLoadPartition);
	  LoadInitrd(OSLoadPartition, params[1], &nargc, nargv);
	}

#if DEBUG
	printf("Command line after initrd: \n\r");
	printCmdLine(nargc, nargv);
	printf("Kernel entry: 0x%lx %lx\n\r",
		(long)(kernel_entry64>>32),(long)(kernel_entry64&0xffffffff));
	Wait("\n\r--- Debug: press <spacebar> to boot kernel ---");
#endif
	if( kernel_entry64 ) {
	    if(is64==0){
		printf("Starting ELF32 kernel\n\r");
		ArcFlushAllCaches();
		((void (*)(int argc, CHAR * argv[], CHAR * envp[]))
		    kernel_entry32)(nargc ,nargv, envp);
	    } else {
		printf("Starting ELF64 kernel\n\r");
		ArcFlushAllCaches();
		_start64(nargc, nargv, &kernel_entry64);
	    }
	} else {
		printf("Invalid kernel entry NULL\n\r");
	}

	/* Not likely to get back here in a functional state, but what the heck */
	Wait("\n\r--- Press <spacebar> to restart ---");
	ArcRestart();
}
