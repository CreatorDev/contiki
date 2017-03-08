CONTIKI_PROJECT = test-proximity
CONTIKI = ../../../..

PROXIMITY_CLICK = proximity-click

TARGET ?= mikro-e

CFLAGS += -DPROXIMITY_CLICK

APPS += proximity-click
APPDIRS += ../../dev

all: $(CONTIKI_PROJECT)

distclean: cleanall

cleanall:
	rm -f symbols.*

include $(CONTIKI)/Makefile.include
