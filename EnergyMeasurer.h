#ifndef ENERGYMEASURER_H
#define ENERGYMEASURER_H

typedef struct EnergyMeasurer {
    void (*start)(struct EnergyMeasurer*);
    void (*stop)(struct EnergyMeasurer*);
    void (*reset)(struct EnergyMeasurer*);
} EnergyMeasurer;

#endif