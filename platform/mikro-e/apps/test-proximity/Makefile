CONTIKI_PROJECT = test-proximity
CONTIKI = ../../../..

PROXIMITY_CLICK = proximity-click

ifndef TARGET
TARGET = mikro-e
endif

CFLAGS += -DPROXIMITY_CLICK

APPS += proximity-click
APPDIRS += ../../dev

all: $(CONTIKI_PROJECT)

distclean: cleanall

cleanall:
	rm -f symbols.*

include $(CONTIKI)/Makefile.include
