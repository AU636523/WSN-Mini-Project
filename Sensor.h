#ifndef SENSOR_H
#define SENSOR_H

#include "MeasurementContainer.h"

typedef struct Sensor {
    munit (*getMeasurement)(struct Sensor*);
    struct measurementContainer *_c;
} Sensor;

#endif