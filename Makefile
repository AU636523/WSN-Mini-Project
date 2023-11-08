CONTIKI_PROJECT = WSN
CFLAGS += -DPROJECT_CONF_H=\"project-conf.h\"

all: $(CONTIKI_PROJECT)

CONTIKI = ../..
TARGET_LIBFILES = -lm

#Add src dir
REST_RESOURCES_DIR = ./src ../../arch/dev/sensor/sht11 
PROJECTDIRS += $(REST_RESOURCES_DIR) 

#Add files in src dir
REST_RESOURCES_FILES = $(notdir $(shell find $(REST_RESOURCES_DIR) -name '*.c'))
PROJECT_SOURCEFILES += $(REST_RESOURCES_FILES)

#Sensor deps
MODULES += dev/sensor/sht11 

#MAKE_MAC = MAKE_MAC_NULLMAC
#MAKE_NET = MAKE_NET_NULLNET
#MAKE_ROUTING = MAKE_ROUTING_NULLROUTING

include $(CONTIKI)/Makefile.include