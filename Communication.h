#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include "project-conf.h"

typedef unsigned char byte;

typedef struct Communication {
    void (*init)(struct Communication*, void*);
    int (*send)(struct Communication*, byte* data, int length);
    int (*incomming_msgs_waiting)(struct Communication*);
    byte* (*getBuffer)(struct Communication*);
    byte buf[COMMUNICATION_BUFFER_BYTE_SIZE];
} Communication;

#endif