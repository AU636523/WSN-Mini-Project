#ifndef MEASUREMENTCONTAINER_H
#define MEASUREMENTCONTAINER_H

#include "project-conf.h"
#include "log.h"

#define LOG_MODULE "MeasurementContainer"
#define LOG_LEVEL LOG_LEVEL_DBG
#include <stdint.h>

typedef uint16_t munit; //Measurement Unit

typedef struct measurementContainer {
    int (*put)(struct measurementContainer*, munit in);
    int (*noBuffersFull)(struct measurementContainer*);
    munit* (*getBatch)(struct measurementContainer*);
    munit batch[TOTAL_BUFFERS][SEND_BUFFER_SIZE];
    uint8_t _batchPtr;
} measurementContainer;

#endif