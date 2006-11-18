#
# Copyright 1999 Silicon Graphics, Inc.
#
CFLAGS += -O2 -Werror -Wall -mno-abicalls -G 0 -fno-pic

TARGETS = libarc.a
OBJECTS = arc.o	stdio.o stdlib.o string.o

all: $(TARGETS)

$(TARGETS): $(OBJECTS)
	rm -f $@
	$(AR) -crs $@ $(OBJECTS)

install: $(TARGETS)
	install -d ${PREFIX}/${LIBDIR}
	install -m 644 $< ${PREFIX}/${LIBDIR}

clean:
	rm -f libarc.a $(OBJECTS) *~ tags
