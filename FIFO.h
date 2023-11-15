#ifndef FIFO_H
#define FIFO_H

#include "MeasurementContainer.h"

void FIFO_init(struct measurementContainer *f);
int FIFO_put(struct measurementContainer *f, munit in); 
munit* FIFO_getBatch(struct measurementContainer *f); 

#endif