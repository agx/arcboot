# Generated automatically from Makefile.in by configure.
#
# Makefile for lib/et
#

srcdir = .
top_srcdir = ../..
top_builddir = ../..
my_dir = lib/et
INSTALL = /usr/bin/install -c

DEP_MAKEFILE = $(DEP_LIB_MAKEFILES)


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
CFLAGS = -g -O2
CPPFLAGS = 
ALL_CFLAGS = $(CPPFLAGS) $(DEFS) $(USE_WFLAGS) $(CFLAGS) $(XTRA_CFLAGS) \
	-I$(top_builddir)/lib -I$(top_srcdir)/lib \
	-I$(top_srcdir)/include $(LINUX_INCLUDE) 
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
# Use these definitions is you use tools 2.x, x < 16
#
#DLL_BIN=/usr/dll/bin
#JUMP_PREFIX=/usr/dll/jump/

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
SUBST_CONF=../util/subst.conf
SUBSTITUTE= ../util/subst -f $(SUBST_CONF)
DEP_SUBSTITUTE= ../util/subst $(SUBST_CONF)

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

all:: compile_et

OBJS= error_message.o et_name.o init_et.o com_err.o
SRCS = $(srcdir)/error_message.c $(srcdir)/et_name.c $(srcdir)/init_et.c \
	$(srcdir)/com_err.c

HFILES= com_err.h
SHARE_FILES= et_c.awk et_h.awk

LIBRARY= libcom_err
LIBDIR= et

DLL_ADDRESS = 0x66800000
DLL_JUMPSIZE = 0x1000
DLL_GOTSIZE  = 0x1000
DLL_VERSION = 1.0
DLL_IMAGE = libet
DLL_STUB = libcom_err
DLL_MYDIR = et
DLL_INSTALL_DIR = $(root_libdir)

ELF_VERSION = 2.0
ELF_SO_VERSION = 2
ELF_IMAGE = libcom_err
ELF_MYDIR = et
ELF_INSTALL_DIR = $(root_libdir)
ELF_OTHER_LIBS = 

BSDLIB_VERSION = 1.0
BSDLIB_IMAGE = libcom_err
BSDLIB_MYDIR = et
BSDLIB_INSTALL_DIR = $(root_libdir)

#
# what to build...
#
.c.o:
	$(CC) $(ALL_CFLAGS) -c $< -o $@
#	$(CC) $(ALL_CFLAGS) -g -pg -o profiled/$*.o -c $<
#	$(CC) $(ALL_CFLAGS) -checker -g -o checker/$*.o -c $<
#	(export JUMP_DIR=`pwd`/jump; $(CC) -B$(JUMP_PREFIX) $(ALL_CFLAGS) \
#		-o jump/$*.o -c $<)
#	$(CC) $(ALL_CFLAGS) -fPIC -o elfshared/$*.o -c $<
#	$(CC) $(ALL_CFLAGS) -fpic -o pic/$*.o -c $<


all:: subdirs $(LIBRARY).a

install-shlibs::

subdirs::

$(LIBRARY).a: $(OBJS)
	(if test -r $@; then $(RM) -f $@.bak && $(MV) $@ $@.bak; fi)
	$(ARUPD) $@ $(OBJS)
	-$(RANLIB) $@
	$(RM) -f ../$@
	$(LN) $@ ../$@







compile_et: $(DEP_SUBSTITUTE) $(srcdir)/compile_et.sh.in
	$(SUBSTITUTE) $(srcdir)/compile_et.sh.in compile_et
	$(CHMOD) +x compile_et

com_err.ps : com_err.dvi
com_err.dvi: com_err.texinfo

#libcom_err.o:	$(LIBOBJS)
#	$(LD) -r -s -o libcom_err.o $(LIBOBJS)
#	chmod -x libcom_err.o

TAGS:	$(SRCS)
	$(TAGS) $(SRCS)

installdirs::
	$(top_srcdir)/mkinstalldirs $(DESTDIR)$(libdir) \
		$(DESTDIR)$(includedir)/et $(DESTDIR)$(datadir)/et \
		$(DESTDIR)$(bindir) $(DESTDIR)$(man1dir) \
		$(DESTDIR)$(man3dir)

install:: compile_et libcom_err.a $(HFILES) installdirs
	$(INSTALL_DATA) libcom_err.a $(DESTDIR)$(libdir)/libcom_err.a
	$(CHMOD) 644 $(DESTDIR)$(libdir)/libcom_err.a
	-$(RANLIB) $(DESTDIR)$(libdir)/libcom_err.a
	$(CHMOD) $(LIBMODE) $(DESTDIR)$(libdir)/libcom_err.a
	for i in $(HFILES); do \
		$(INSTALL_DATA) $(srcdir)/$$i $(DESTDIR)$(includedir)/et/$$i; \
	done
	for i in $(SHARE_FILES); do \
		$(INSTALL_DATA) $(srcdir)/$$i $(DESTDIR)$(datadir)/et/$$i; \
	done
	$(INSTALL) compile_et $(DESTDIR)$(bindir)/compile_et
	$(INSTALL_DATA) $(srcdir)/com_err.3 $(DESTDIR)$(man3dir)/com_err.3
	$(INSTALL_DATA) $(srcdir)/compile_et.1 \
			$(DESTDIR)$(man1dir)/compile_et.1

uninstall::
	$(RM) -f $(DESTDIR)$(libdir)/libcom_err.a \
			$(DESTDIR)$(bindir)/compile_et
	$(RM) -rf $(DESTDIR)$(includedir)/et $(DESTDIR)$(datadir)/et

clean::
	$(RM) -f compile_et libcom_err.a libcom_err_p.a
	$(RM) -f $(OBJS) profiled/*
	$(RM) -f *~ \#* *.bak *.otl *.aux *.toc *.PS *.dvi *.ps TAGS *.ln
	$(RM) -f ../libcom_err.a ../libcom_err_p.a
mostlyclean:: clean
distclean:: clean
	$(RM) -f .depend Makefile $(srcdir)/TAGS $(srcdir)/Makefile.in.old

# +++ Dependency line eater +++
# 
# Makefile dependencies follow.  This must be the last section in
# the Makefile.in file
#
error_message.o: $(srcdir)/error_message.c $(srcdir)/com_err.h \
 $(srcdir)/error_table.h $(srcdir)/internal.h
et_name.o: $(srcdir)/et_name.c $(srcdir)/com_err.h $(srcdir)/error_table.h \
 $(srcdir)/internal.h
init_et.o: $(srcdir)/init_et.c $(srcdir)/com_err.h $(srcdir)/error_table.h
com_err.o: $(srcdir)/com_err.c $(srcdir)/com_err.h $(srcdir)/error_table.h \
 $(srcdir)/internal.h
