#
# Copyright 1999 Silicon Graphics, Inc.
#           2001-04 Guido Guenther <agx@sigxcpu.org>
#

SUBARCH ?= IP22

COMMONDIR = ../common

E2FSINCLUDEDIR ?= /usr/include/ext2fs
E2FSLIBDIR ?= /usr/lib
EXT2LIB ?= $(E2FSLIBDIR)/libext2fs-nopic.a

ARCINCLUDEDIR = ../arclib
ARCLIBDIR = ../arclib
ARCLIB = $(ARCLIBDIR)/libarc.a

OBJECTS = loader.o ext2io.o conffile.o
LIBS = $(EXT2LIB) $(ARCLIB)
TARGETS = ext2load 

CFLAGS = -O2 -I$(COMMONDIR) -I$(ARCINCLUDEDIR) -I$(E2FSINCLUDEDIR) 	\
         -Wextra -Wall -mno-abicalls -G 0 -fno-pic			\
	 -DSUBARCH=${SUBARCH} -fno-strict-aliasing

# uncomment for debugging
#CFLAGS+=-DDEBUG

LD = ld
LDFLAGS = -N -T ld.script

all:  $(TARGETS)

ext2load:  $(OBJECTS) $(LIBS) ld.script ../common/subarch.h
	rm -f $@
	$(LD) $(LDFLAGS) -o $@ $(OBJECTS) $(LIBS)

ld.script: ld.script.in
	$(MAKE) -C ../common SUBARCH=$(SUBARCH)
	LOADADDR=$(LOADADDR);	 				\
	OUTPUTFORMAT=$(OUTPUTFORMAT);				\
	sed -e "s/@@LOADADDR@@/$$LOADADDR/"			\
	-e "s/@@OUTPUTFORMAT@@/$$OUTPUTFORMAT/" <$< >$@

install:

clean:
	rm -f $(TARGETS) *.a *.o tags ld.script
