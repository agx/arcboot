SUBARCH ?= IP22

CFLAGS += -Wall -O2 -I. -I../arclib -DSUBARCH=${SUBARCH}

HOSTCC = $(CC)
HOSTCFLAGS += -Wall -O2 -DSUBARCH=$(SUBARCH)

all:

clean:
	rm -f  *~ tags
