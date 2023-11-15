#ifndef NULLAGGREGATOR_H
#define NULLAGGREGATOR_H

#include "DataAggregator.h" 

void nullAggregator_init(DataAggregator *a);
uint8_t nullAggregator_do_agg(struct DataAggregator *a, munit* in, byte *out);

#endif