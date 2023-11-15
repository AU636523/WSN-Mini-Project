#include "nullSensor.h"

void nullSensor_init(struct Sensor *s) {
    s->getMeasurement = nullSensor_getMeasurement;
    return;
}

munit nullSensor_getMeasurement(struct Sensor *s) {
    return 0;
}