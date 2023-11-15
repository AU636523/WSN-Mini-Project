#include "nullAggregator.h"

void nullAggregator_init(struct DataAggregator *a) {
    a->init = nullAggregator_init;
    a->do_agg = nullAggregator_do_agg;
    return;
}

uint8_t nullAggregator_do_agg(struct DataAggregator *a, munit* in, byte *out) {
    return 0; 
}