#ifndef SENSOR_H
#define SENSOR_H

#include "MeasurementContainer.h"

typedef struct Sensor {
    munit (*getMeasurement)(struct Sensor*);
    struct measurementContainer *_c;
} Sensor;

void randSensor_init(Sensor *s);
munit randSensor_getMeasurement(struct Sensor *s);



#endif