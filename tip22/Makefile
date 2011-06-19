#
# Copyright 2002-2004,2011 Guido Guenther <agx@sigxcpu.org>
#

ARCLIBDIR = ../arclib
ARCLIB = $(ARCLIBDIR)/libarc.a

COMMONDIR = ../common

MIPS_CFLAGS = -mno-abicalls -G 0 -fno-pic
WARN_CFLAGS = -Wextra -Wall
# uncomment for debugging
#DEBUG_CFLAGS = -DDEBUG
CFLAGS += -O2 -I$(COMMONDIR) -I$(ARCLIBDIR) \
	  -DSUBARCH=${SUBARCH} -fno-strict-aliasing \
	  $(WARN_CFLAGS) $(MIPS_CFLAGS) $(DEBUG_CFLAGS)

ASFLAGS = -O2 -mno-abicalls -G 0 -fno-pic

LIBDIR ?= /usr/lib/tip22
BINDIR ?= /usr/sbin

LIBS=${ARCLIB}
BINS=tip22 tip32
LD_SCRIPTS = ld.kernel.script.$(SUBARCH) ld.ramdisk.script.$(SUBARCH) ld.script.$(SUBARCH)
OBJECTS = tftpload.$(SUBARCH).o
TARGETS = $(OBJECTS) $(LD_SCRIPTS)

# uncomment for debugging
#CFLAGS+=-DDEBUG

all: ${LIBS} ${BINS} $(TARGETS)

$(ARCLIB):
	@$(MAKE) -C $(ARCLIBDIR)

%.$(SUBARCH).o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.script.$(SUBARCH): %.script.in
	LOADADDR=$(TIP_LOADADDR);				\
	OUTPUTFORMAT=$(OUTPUTFORMAT);				\
	sed -e "s/@@LOADADDR@@/$$LOADADDR/"			\
	-e "s/@@OUTPUTFORMAT@@/$$OUTPUTFORMAT/" <$< >$@

clean:
	rm -f tags *~
	rm -f $(TARGETS)

install: all
	install -d ${PREFIX}/${BINDIR}
	install -m 755 ${BINS} ${PREFIX}/${BINDIR}
	$(foreach tg,$(TARGETS),install -m 644 $(tg) ${PREFIX}/${LIBDIR};)


.PHONY: all archall clean archclean install archinstall
