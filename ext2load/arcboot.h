/*
 * Copyright 2001-03 Guido Guenther <agx@sigxcpu.org>
 */

#ifndef _ARCBOOT_H
#define _ARCBOOT_H

#include <version.h>

/* loader.c */
extern CHAR *OSLoadPartition;
extern CHAR *OSLoadFilename;
extern CHAR *OSLoadOptions;

typedef enum { False = 0, True } Boolean;

Boolean OpenFile(const char *partition, const char *filename, ext2_file_t* file);
void Fatal(const CHAR * message, ...);

/* conffile.c */
CHAR** ReadConfFile(char **partition, const char *filename, char* config);

/* ext2io.c */
extern int arc_do_progress;
void print_ext2fs_error(long status);
#endif /* _ARCBOOT_H */
