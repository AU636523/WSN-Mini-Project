CONTIKI_PROJECT = WSN

all: $(CONTIKI_PROJECT)
CONTIKI = ../..

#Add src dir
REST_RESOURCES_DIR = .
PROJECTDIRS += $(REST_RESOURCES_DIR) 

#Add files in src di
REST_RESOURCES_FILES = $(shell find *.c)
PROJECT_SOURCEFILES += $(REST_RESOURCES_FILES)

include $(CONTIKI)/Makefile.include
