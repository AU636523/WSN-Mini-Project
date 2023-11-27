#ifndef MEASUREMENTCONTAINER_H
#define MEASUREMENTCONTAINER_H

#include "project-conf.h"

#include <stdint.h>

typedef struct measurementContainer {
    int (*put)(struct measurementContainer*, munit in);
    munit* (*getBatch)(struct measurementContainer*);
    munit container[SEND_BUFFER_SIZE];
    uint8_t _inIdx;
} measurementContainer;


#endif