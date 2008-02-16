/*
 * subarch specific definitions
 */

#ifndef SUBARCH_H
#define SUBARCH_H

#include <stdint.h>

#define PAGE_SIZE	4096
#define STACK_PAGES	16

/* supported subarches */
#define IP22 	0
#define IP32 	1

/*
 *  Reserve this memory for loading kernel
 *  Don't put loader structures there because they would be overwritten
 *
 *  We put the loader right after the kernel so you won't have the
 *  full reserved space since the prom puts the stack right below
 *  the loader.
 */
struct kernel_load_block {
	uint32_t base;
	uint32_t reserved;
};

struct kernel_load_block kernel_load[] = {
	{ /* IP22 */
	.base =     0x88002000,
	.reserved =  0x1700000,
	},
	{ /* IP32 */
	.base     = 0x80004000,
	.reserved =  0x1400000,
	},
};

/* we filter these out of the command line */
char* env_vars[] = { "ConsoleIn=",
                     "ConsoleOut=",
                     "OSLoader=",
                     "OSLoadPartition=",
                     "OSLoadFilename=",
                     "OSLoadOptions=",
                   };
#define NENTS(foo) ((sizeof((foo)) / (sizeof((foo[0])))))

#endif
