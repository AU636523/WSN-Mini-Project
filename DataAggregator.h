#ifndef DATAAGGREGATOR_H
#define DATAAGGREGATOR_H

#include "MeasurementContainer.h"
#include "Communication.h"

typedef struct DataAggregator {
    void (*init)(struct DataAggregator*);
    uint8_t (*do_agg)(struct DataAggregator*, munit*, byte*);
} DataAggregator;

void dataAggregator_init(DataAggregator *a);

#endif