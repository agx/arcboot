/*
 * Copyright 2001-2004 Guido Guenther <agx@sigxcpu.org>
 *
 * load arcboots configuration file and process the arguments
 *
 */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <types.h>

#include <sys/types.h>
#include <ext2_fs.h>
#include <ext2fs.h>
#include "arcboot.h"
#include <arc.h>

#define _PARM_LIMIT	32

static	char	*carray[_PARM_LIMIT+3]; /* 0 is the name, 
					   1 the boofile, ...
					   X is OSLoadOptions 
					   X+1 ... _PARM_LIMIT are options given on the
					   command line */

CHAR** GetConfig(char* config, char* name)
{
	char *t, *start, *end;
	int		i;

	/* Loop on lines */
	while(*config != 0x0) {

		start=config;

		while(*config != 0xa && *config != 0x0) {
			/* Delete comments */
			if (*config == '#') 
				*config=0x0;
			config++;
		}

		/* Did we stop at the end of a line ? */
		if (*config == 0xa) {
			/* Terminate Line */
			*config=0x0;
			config++;
		}		

		/* Skip leading spaces and tabs */
		while(*start == ' ' || *start == '\t') 
			start++;

		/* If the start of a line is the end - Next line */
		if (*start == 0x0)
			continue;

		/* get the end pointer */
		end=&start[strlen(start)-1];

		/* Delete spaces and tabs at the end of a line */
		while(*end == ' ' || *end == '\t')
			*end--=0x0;

		if (strncmp("label=",start,6) == 0) {
			/* If we found the right profile or want the first */
			if (carray[0])
				if (((strcmp(carray[0], name) == 0) && (strcmp(name, carray[0]) == 0))) {
					return carray;
				}
			/* Reset image & append */
			carray[1]=carray[2]=0;
			carray[0]=&start[6];
		} else if (strncmp("image=",start,6) == 0) {
			carray[1]=&start[6];
		} else if (strncmp("append=",start,7) == 0) {
			t=&start[7];
			/* Does append start with " */
			if (*t == '"') {
				t++;
				/* If so - append starts +1 */
				carray[2]=t;
				/* Search ending quote */
				while(*t != '"' && *t != 0x0)
					t++;
				/* And delete */
				if (*t == '"') 
					*t=0x0;
			} else 
				carray[2]=&start[7];
			t=carray[2];
			i=3;
			while(i<_PARM_LIMIT && *t != 0x0) {
				t++;
					
				if (*t == ' ' || *t == '\t') {
					*t++=0x0;
					if (*t != 0x0)
						carray[i++]=t;
				}
			}
		}
	}
	if (carray[0])
		if ((name == NULL) ||
			(strcmp(carray[0], name) == 0)) {
			return carray;
		}
	/* Found nothing appropriate: */
	return NULL;
}

CHAR** ReadConfFile(char** partition, const char *filename, char* label)
{
	ext2_file_t file;
	unsigned size, num_read;
	errcode_t status;
	char *conf_file;

	if(!OpenFile( *partition, filename, &file )){
		/* OSLoadPartition seems to be wrong, but don't give up now */
		int npart,i;
		char *part,*spart;

		/* the user wants to boot a file directly out of the filesystem
		 * don't try to fixup OSLoadPartition for him in this case */
		if(label[0] == '/') {
			return False;
		}
		printf("Can't open configuration file. Trying other partitions\n\r");
		spart = ArcGetEnvironmentVariable("SystemPartition");
		if(! spart ) {
			printf("Couldn't get SystemPartition, weird.");
			return False;
		}
		part = strdup(spart);
		npart = part[strlen(part)-2] - '0';
		for(i = 0; i < npart; i++) {
			part[strlen(part)-2] = '0' + i;
#if DEBUG
			printf("Trying %s\n\r", part);
#endif
			/* we found it, good */
			if(OpenFile( part, filename, &file )) {
				printf("Please adjust OSLoadPartition to %s\n\r", part);
				*partition = part;
				break;
			}
		}
		if( i == npart )
			return False;
	}

	size = ext2fs_file_get_size(file);
	conf_file = malloc(size);
	if( !conf_file ) {
		printf("Can't read configuration file - not enough memory\n\r");
		return False;
	}
	status = ext2fs_file_read(file,(char*) conf_file, size, &num_read);
	if( status ) {
		print_ext2fs_error(status);
		return False;
	}
	if( size != num_read ) {
		printf("Wanted: %u, got %u bytes of configuration file\n\r", size, num_read);
		return False;
	}
	return GetConfig(conf_file, label); 
}
