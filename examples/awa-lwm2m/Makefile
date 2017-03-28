CONTIKI_PROJECT=awa-lwm2m
CONTIKI=../..
TARGET?=mikro-e
VERSION?=0.2.5

ifeq ($(TARGET),micro-e)
PROJECT_CONF_H:=project-conf.h
all: $(CONTIKI_PROJECT).hex
else
PROJECT_CONF_H:=project-conf-$(TARGET).h
endif

ifeq ($(TARGET),minimal-net)
ifndef TAP_PREFIX
$(error Please define TAP_PREFIX. See HARD_CODED_ADDRESS at https://github.com/contiki-os/contiki/blob/master/platform/minimal-net/contiki-conf.h)
endif
UIP_CONF_RPL=0
CFLAGS += -DUIP_CONF_ND6_DEF_MAXDADNS=0
CFLAGS += '-DHARD_CODED_ADDRESS="$(TAP_PREFIX)"'
CFLAGS += -DDTLS_MAX_BUF=1000
#CFLAGS += -DVERBOSE_DEBUG=1 -DDEBUG=1
endif

CFLAGS += '-DPROJECT_CONF_H="$(PROJECT_CONF_H)"' '-DVERSION="$(VERSION)"'
CFLAGS += -Wall -Wno-pointer-sign
CFLAGS += -DLWM2M_CLIENT -g -DDTLS_PEERS_NOHASH=1
CFLAGS += -DUIP_CONF_BUFFER_SIZE=4096

ifeq ($(PSK_IDENTITY)x,x)
$(error Please define PSK_IDENTITY)
endif
ifeq ($(PSK_SECRET_KEY)x,x)
$(error Please define PSK_SECRET_KEY)
endif
PSK_BINARY_KEY:=$(shell echo $(PSK_SECRET_KEY) | sed -e 's.\([0-9A-F][0-9A-F]\).0x\1 .g' -e 's. 0x.,0x.g')

$(CONTIKI_PROJECT).co: CFLAGS += '-DIDENTITY="$(PSK_IDENTITY)"' '-DSECRET_KEY="$(PSK_SECRET_KEY)"' '-DBINARY_KEY=$(PSK_BINARY_KEY)'

APPS += awa-lwm2m
APPS += tinydtls/aes tinydtls/sha2 tinydtls/ecc tinydtls

CONTIKI_WITH_IPV6 = 1
CONTIKI_WITH_RPL = 0
RESOLV_CONF_SUPPORTS_MDNS = 0
UIP_CONF_TCP = 1
TINYDTLS = 1
USE_SERIAL_PADS = 0

CFLAGS += -I $(CONTIKI)/platform/$(TARGET)
CFLAGS += -fno-short-double
LDFLAGS += -Wl,--defsym,_min_heap_size=64000

SMALL=0

all: $(CONTIKI_PROJECT)

$(CONTIKI_PROJECT).hex: $(CONTIKI_PROJECT)
	xc32-bin2hex $(CONTIKI_PROJECT).$(TARGET)

include $(CONTIKI)/Makefile.include
