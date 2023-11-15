#ifndef NULLMEASURER_H
#define NULLMEASURER_H

#include "EnergyMeasurer.h"

void nullMeasurer_init(EnergyMeasurer *m);
void nullMeasurer_start(struct EnergyMeasurer *m);

void nullMeasurer_stop(struct EnergyMeasurer *m);

void nullMeasurer_reset(struct EnergyMeasurer *m);

#endif