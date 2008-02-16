/*
 * Copyright 2002-04 Guido Guenther <agx@sigxcpu.org>
 * 
 * based on arcboot/ext2load/loader.c
 *
 */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <types.h>

#include <arc.h>
#include <elf.h>

#include <sys/types.h>

#include <version.h>
#include <subarch.h>

#define KSEG0ADDR(addr)	(((addr) & 0x1fffffff) | 0x80000000)

#define ANSI_CLEAR	"\033[2J"

typedef enum { False = 0, True } Boolean;

extern void* __kernel_start;
extern void* __kernel_end;
extern void* __rd_start;
extern void* __rd_end;

static int is64 = 0;

static void Wait(const char *prompt)
{
	int ch;

	if (prompt != NULL)
		puts(prompt);

	do {
		ch = getchar();
	} while ((ch != EOF) && (((char) ch) != ' '));
}


static void Fatal(const CHAR * message, ...)
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


static void InitMalloc(void)
{
	MEMORYDESCRIPTOR *current = NULL;
	ULONG stack = (ULONG) & current;
#ifdef DEBUG
	printf("stack starts at: 0x%lx\n\r", stack);
#endif

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
#if DEBUG
			printf("Free Memory(%u) segment found at (0x%lx,0x%lx).\n\r",
					current->Type, start, end); 
#endif

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
#ifdef DEBUG
				printf("Adding %lu bytes at 0x%lx to the list of available memory\n\r", 
						end-start, start);
#endif
				arclib_malloc_add(start, end - start);
			}
		}
		current = ArcGetMemoryDescriptor(current);
	}
}

/* convert an offset in the kernel image to an address in the loaded tftpboot image */
static void* offset2addr(unsigned long offset)
{
	void* address = (void*)((ULONG)&(__kernel_start) + offset);
	return address;
}

/* copy program segments to the locations the kernel expects */
static ULONG CopyProgramSegments32(Elf32_Ehdr * header)
{
	int idx;
	Boolean loaded = False;
	Elf32_Phdr *segment, *segments;
	size_t size = header->e_phentsize * header->e_phnum;
	ULONG kernel_end=0L;

	if (size <= 0)
		Fatal("No program segments\n\r");

	segments = malloc(size);
	if (segments == NULL)
		Fatal("Cannot allocate memory for segment headers\n\r");

        segments = (Elf32_Phdr*)offset2addr(header->e_phoff);

	segment = segments;
	for (idx = 0; idx < header->e_phnum; idx++) {
		if (segment->p_type == PT_LOAD) {
			printf
			    ("Loading program segment %u at 0x%x, size = 0x%x\n\r",
			     idx + 1, KSEG0ADDR(segment->p_vaddr), segment->p_filesz);

			memcpy((void *)segment->p_vaddr, offset2addr(segment->p_offset), segment->p_filesz);
			/* determine the highest address used by the kernel's memory image */
			if( kernel_end < segment->p_vaddr + segment->p_memsz ) {
				kernel_end = segment->p_vaddr + segment->p_memsz;
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
			loaded = True;
		}
		segment =
		    (Elf32_Phdr *) (((char *) segment) +
				    header->e_phentsize);
	}

	if (!loaded)
		Fatal("No loadable program segments found\n\r");

	free(segments);
	return kernel_end;
}

static ULONG CopyProgramSegments64(Elf64_Ehdr * header)
{
	int idx;
	Boolean loaded = False;
	Elf64_Phdr *segment, *segments;
	ULONG size = header->e_phentsize * header->e_phnum;
	ULONG kernel_end=0L;

	if (size <= 0)
		Fatal("No program segments\n\r");

	segments = malloc(size);
	if (segments == NULL)
		Fatal("Cannot allocate memory for segment headers\n\r");

        segments = (Elf64_Phdr*)offset2addr(header->e_phoff);

	segment = segments;
	for (idx = 0; idx < header->e_phnum; idx++) {
		if (segment->p_type == PT_LOAD) {
			printf ("Loading program segment %u at 0x%x, "
				"size = 0x%lx %lx\n\r",
				idx + 1,
				(int)KSEG0ADDR(segment->p_vaddr),
				(long)(segment->p_filesz>>32),
				(long)(segment->p_filesz&0xffffffff));

			memcpy((void *)(long)(segment->p_vaddr), offset2addr(segment->p_offset), segment->p_filesz);
			/* determine the highest address used by the kernel's memory image */
			if( kernel_end < segment->p_vaddr + segment->p_memsz ) {
				kernel_end = segment->p_vaddr + segment->p_memsz;
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
			loaded = True;
		}
		segment =
		    (Elf64_Phdr *) (((char *) segment) +
				    header->e_phentsize);
	}

	if (!loaded)
		Fatal("No loadable program segments found\n\r");

	free(segments);
	return kernel_end;
}

static Elf64_Addr CopyKernel(ULONG *kernel_end)
{
	Elf32_Ehdr *header = (Elf32_Ehdr*)offset2addr(0L);
	Elf64_Ehdr *header64 = (Elf64_Ehdr*)header;

	if (memcmp(&(header->e_ident[EI_MAG0]), ELFMAG, SELFMAG) != 0)
		Fatal("Not an ELF file\n\r");

	if (header->e_ident[EI_CLASS] == ELFCLASS32) {
		if (header->e_ident[EI_DATA] != ELFDATA2MSB)
			Fatal("Not a big-endian file\n\r");
		if (header->e_ident[EI_VERSION] != EV_CURRENT)
			Fatal("Wrong ELF version\n\r");
		if (header->e_type != ET_EXEC)
			Fatal("Not an executable file\n\r");
		if (header->e_machine != EM_MIPS)
			Fatal("Unsupported machine type\n\r");
		if (header->e_version != EV_CURRENT)
			Fatal("Wrong ELF version\n\r");

		(*kernel_end) = CopyProgramSegments32(header);

		printf("ELF32 kernel entry point = 0x%lx\n\r", (ULONG)header->e_entry);
		return (Elf64_Addr) header->e_entry;
	} else if (header->e_ident[EI_CLASS] == ELFCLASS64) {
		is64 = 1;

		if (header64->e_ident[EI_DATA] != ELFDATA2MSB)
			Fatal("Not a big-endian file\n\r");
		if (header64->e_ident[EI_VERSION] != EV_CURRENT)
			Fatal("Wrong ELF version\n\r");
		if (header64->e_type != ET_EXEC)
			Fatal("Not an executable file\n\r");
		if (header64->e_machine != EM_MIPS)
			Fatal("Unsupported machine type\n\r");
		if (header64->e_version != EV_CURRENT)
			Fatal("Wrong ELF version\n\r");

		(*kernel_end) = CopyProgramSegments64(header64);

		printf("ELF64 kernel entry point = 0x%lx %lx\n\r",
		       (ULONG)(header64->e_entry >> 32), (ULONG)(header64->e_entry & 0xffffffff));
		return header64->e_entry;
	} else
		Fatal("Neither an ELF32 nor an ELF64 kernel\n\r");

	return 0L;
}

static void copyRamdisk(void* rd_vaddr, void* rd_start, ULONG rd_size)
{
	printf("Copying initrd from 0x%p to 0x%p (0x%lx bytes)...\n\r", 
			rd_start, rd_vaddr, rd_size);
	memcpy(rd_vaddr, rd_start, rd_size);
	printf("Initrd copied.\n\r");
}

void _start64(LONG argc, CHAR * argv[], CHAR * envp[],
              unsigned long long *addr)
{
  __asm__ __volatile__(
		       ".set push\n"
		       "\t.set mips3\n"
		       "\t.set noreorder\n"
		       "\t.set noat\n"
		       "\tld $1, 0($7)\n"
		       "\tjr $1\n"
		       "\t nop\n"
		       "\t.set pop");
}

void _start(LONG argc, CHAR * argv[], CHAR * envp[])
{
	char* nargv[3];
	int nargc,i;
	char argv_rd[128];	/* passed to the kernel on its commandline */
	ULONG kernel_end = 0L;
	ULONG rd_size= ((char*)&__rd_end) - ((char*)&__rd_start);
	ULONG rd_vaddr;
	Elf32_Addr kernel_entry32;
	Elf64_Addr kernel_entry64;

	/* Print identification */
#if (SUBARCH == IP22)
	printf(ANSI_CLEAR "\n\rtip22: IP22 Linux tftpboot loader " __ARCSBOOT_VERSION__ "\n\r");
#elif (SUBARCH == IP32)
	printf(ANSI_CLEAR "\n\rtip32: IP32 Linux tftpboot loader " __ARCSBOOT_VERSION__ "\n\r");
#endif

	InitMalloc();

	/* copy kernel and ramdisk to its load addresses */
#ifdef DEBUG
	printf("Embedded kernel image starts 0x%p, ends 0x%p\n\r", 
			&__kernel_start, &__kernel_end);
	printf("Embedded ramdisk image starts 0x%p, ends 0x%p\n\r", 
			&__rd_start, &__rd_end);
#endif
	kernel_entry64 = CopyKernel(&kernel_end);
	kernel_entry32 = (Elf32_Addr) kernel_entry64;

	rd_vaddr = (ULONG)malloc(rd_size + PAGE_SIZE);
	/* align to page boundary */
	rd_vaddr = (rd_vaddr + PAGE_SIZE) & ~(PAGE_SIZE - 1);

#ifdef DEBUG
	printf("rd_start=0x%lx rd_size=0x%lx\n\r", rd_vaddr, rd_size);
#endif

	copyRamdisk( (char *)rd_vaddr, (char*)&__rd_start, rd_size);

	/* tell the kernel about the ramdisk */
	sprintf(argv_rd, "rd_start=0x%lx rd_size=0x%lx", rd_vaddr, rd_size);

	nargv[0] = argv[0];
	nargv[1] = argv_rd;
	nargc=2;
	for(i=1; i < argc; i++) {
		if( !memcmp(argv[i],"append=",7) )
			break;
	}

	if( i < argc ) { /* we're asked to pass s.th. to the kernel */
		nargv[2] = argv[i]+7;
		nargc++;
	}

#ifdef DEBUG
	printf("Arguments passed to kernel:\n\r");
	for(i = 0; i < nargc; i++ )
		printf("%u: %s\n\r", i, nargv[i]);
	Wait("\n\r--- Debug: press <spacebar> to boot kernel ---");
#endif
	/* Finally jump into the kernel */
	if( kernel_entry64 ) {
		if (is64 == 0) {
			printf("Starting ELF32 kernel\n\r");
			ArcFlushAllCaches();
			((void (*)(int argc, CHAR * argv[], CHAR * envp[]))kernel_entry32)(nargc ,nargv, envp);
		} else {
			printf("Starting ELF64 kernel\n\r");
			ArcFlushAllCaches();
			_start64(nargc, nargv, envp, &kernel_entry64);
		}
	} else
		printf("Invalid kernel entry NULL\n\r");

	/* Not likely to get back here in a functional state, 
	 * but what the heck */
	Wait("\n\r--- Press <spacebar> to restart ---");
	ArcRestart();
}
