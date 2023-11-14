CONTIKI_PROJECT = WSN
CFLAGS += -DPROJECT_CONF_PATH=\"project-conf.h\" -DNETWORK_ID=$(NetworkId)

all: $(CONTIKI_PROJECT)
CONTIKI = ../..
TARGET_LIBFILES = -lm

#Add src dir
REST_RESOURCES_DIR = ./src/NetDriver
PROJECTDIRS += $(REST_RESOURCES_DIR) 

#Add files in src dir
REST_RESOURCES_FILES = $(notdir $(shell find $(REST_RESOURCES_DIR) -name '*.c'))
PROJECT_SOURCEFILES += $(REST_RESOURCES_FILES)

MAKE_ROUTING = MAKE_ROUTING_RPL_CLASSIC

include $(CONTIKI)/Makefile.include