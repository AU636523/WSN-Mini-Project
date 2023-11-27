#ifndef NULLSENSOR_H
#define NULLSENSOR_H

#include "Sensor.h"

void randSensor_init(Sensor *s);
munit randSensor_getMeasurement(struct Sensor *s);

#endif