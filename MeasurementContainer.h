#ifndef MEASUREMENTCONTAINER_H
#define MEASUREMENTCONTAINER_H

#include "project-conf.h"
#include "node-id.h"

#include <stdint.h>

typedef struct measurementContainer {
    int     (*put)          (struct measurementContainer*, munit in);
    munit*  (*getBatch)     (struct measurementContainer*);
    
    munit container[MESSAGE_MEASUREMENT_COUNT];
    uint8_t _inIdx;
} measurementContainer;


#endif