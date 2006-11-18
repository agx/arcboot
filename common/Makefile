SUBARCH ?= IP22

CFLAGS += -Wall -O2 -I. -I../arclib -DSUBARCH=${SUBARCH}

HOSTCC = $(CC)
HOSTCFLAGS += -Wall -O2 -DSUBARCH=$(SUBARCH)

all: print_loadaddr

print_loadaddr: print_loadaddr.c subarch.h version.h
	$(HOSTCC) $(HOSTCFLAGS) -o $@ $<

install: print_loadaddr

clean:
	rm -f print_loadaddr *~ tags
