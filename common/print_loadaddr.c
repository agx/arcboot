/* small helper to get the current subarch's loaddr */

#include <stdio.h>
#include <stdint.h>
#include <strings.h>

#include "subarch.h"

int main(int argc, char *argv[])
{
	int subarch = SUBARCH;

	if (argc == 2) {
		if (!strcasecmp(argv[1], "ip22"))
			subarch = IP22;
		else if (!strcasecmp(argv[1], "ip32"))
			subarch = IP32;
		else {
			fprintf(stderr,
				"Unknown subarchitecture %s requested\n",
				argv[1]);
			return 1;
		}
	}

	printf("%#08x\n", kernel_load[subarch].base
	       + kernel_load[subarch].reserved);

	return 0;
}
