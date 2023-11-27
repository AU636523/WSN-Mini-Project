#ifndef CIRCULARCONTAINER_H
#define CIRCULARCONTAINER_H

#include "MeasurementContainer.h"

void circularContainer_init(measurementContainer* m);
int circularContainer_put(struct measurementContainer* m, munit in);
munit* circularContainer_getBatch(struct measurementContainer* m);

#endif