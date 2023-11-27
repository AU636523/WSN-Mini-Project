#include "randSensor.h"

#include "random.h"

#include "sys/log.h"
#define LOG_MODULE "randSensor"
#define LOG_LEVEL LOG_LEVEL_INFO

void randSensor_init(struct Sensor *s) {
    s->getMeasurement = randSensor_getMeasurement;
    random_init(42);
    return;
}

munit randSensor_getMeasurement(struct Sensor *s) {
    int random = random_rand();
    LOG_INFO("Random sensor measurement: %d\n", random);
    return (munit)random;
}