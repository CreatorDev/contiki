CONTIKI_PROJECT = test-relay
CONTIKI = ../../../..

TARGET ?= mikro-e
CFLAGS += -DRELAY_CLICK

APPS += relay-click
APPDIRS += ../../dev

all: $(CONTIKI_PROJECT)

distclean: cleanall

cleanall:
	rm -f symbols.*

include $(CONTIKI)/Makefile.include
