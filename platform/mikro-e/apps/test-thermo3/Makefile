CONTIKI_PROJECT = test-thermo3
CONTIKI = ../../../..
TARGET ?= mikro-e
CFLAGS += -DTHERMO3_CLICK

APPS += thermo3-click
APPDIRS += ../../dev

all: $(CONTIKI_PROJECT)

distclean: cleanall

cleanall:
	rm -f symbols.*

include $(CONTIKI)/Makefile.include
