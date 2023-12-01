#ifndef NULLSENSOR_H
#define NULLSENSOR_H

#include "Sensor.h"

void nullSensor_init(Sensor *s);
munit nullSensor_getMeasurement(struct Sensor *s);

#endif