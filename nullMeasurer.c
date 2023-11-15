#include "nullMeasurer.h"

void nullMeasurer_init(struct EnergyMeasurer *m) {
    m->start = nullMeasurer_start;
    m->stop = nullMeasurer_stop;
    m->reset = nullMeasurer_reset;
    return;
}

void nullMeasurer_start(struct EnergyMeasurer *m) {
}

void nullMeasurer_stop(struct EnergyMeasurer *m) {
}

void nullMeasurer_reset(struct EnergyMeasurer *m) {
}