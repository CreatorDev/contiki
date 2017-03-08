CONTIKI_PROJECT = test-motion
CONTIKI = ../../../..

CFLAGS += -DMOTION_CLICK

TARGET ?= mikro-e

APPS += motion-click
APPDIRS += ../../dev

all: $(CONTIKI_PROJECT)

distclean: cleanall

cleanall:
	rm -f symbols.*

include $(CONTIKI)/Makefile.include
