#
# Copyright 2002-2004 Guido Guenther <agx@sigxcpu.org>
#

ARCLIBDIR = ../arclib
ARCLIB = $(ARCLIBDIR)/libarc.a

COMMONDIR = ../common
PRINT_LOADADDR = $(COMMONDIR)/print_loadaddr
PRINT_OUTPUTFORMAT = $(COMMONDIR)/print_outputformat

CFLAGS += -O2 -I$(COMMONDIR) -I$(ARCLIBDIR) -Wall -mno-abicalls -G 0 \
	-fno-pic -DSUBARCH=${SUBARCH}

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


all: ${LIBS} ${BINS}
	@$(MAKE) SUBARCH=IP32 archall
	@$(MAKE) SUBARCH=IP22 archall

archall: $(TARGETS)


$(ARCLIB):
	@$(MAKE) -C $(ARCLIBDIR)

$(PRINT_LOADADDR):
	@$(MAKE) -C $(COMMONDIR) SUBARCH=$(SUBARCH)


%.$(SUBARCH).o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.script.$(SUBARCH): %.script.in $(PRINT_LOADADDR) $(PRINT_OUTPUTFORMAT)
	LOADADDR=$$($(PRINT_LOADADDR) $(SUBARCH));	 	\
	OUTPUTFORMAT=$$($(PRINT_OUTPUTFORMAT) $(SUBARCH));	\
	sed -e "s/@@LOADADDR@@/$$LOADADDR/"			\
	-e "s/@@OUTPUTFORMAT@@/$$OUTPUTFORMAT/" <$< >$@

clean:
	@$(MAKE) SUBARCH=IP32 archclean
	@$(MAKE) SUBARCH=IP22 archclean
	rm -f tags *~

archclean:
	@$(MAKE) -C $(ARCLIBDIR) clean
	@$(MAKE) -C $(COMMONDIR) clean
	rm -f $(TARGETS)

install: all
	install -d ${PREFIX}/${BINDIR}
	install -m 755 ${BINS} ${PREFIX}/${BINDIR}
	@$(MAKE) SUBARCH=IP32 archinstall
	@$(MAKE) SUBARCH=IP22 archinstall

archinstall:
	$(foreach tg,$(TARGETS),install -m 644 $(tg) ${PREFIX}/${LIBDIR};)


.PHONY: all archall clean archclean install archinstall
