#ifndef MSGFORMAT_H
#define MSGFORMAT_H

#include "MeasurementContainer.h"

typedef unsigned char byte;

#define BIT_SET(x,n) ((x) |= (1 << (n)))
#define IS_BIT_SAT(x,n) ((x) & (1 << (n)))

typedef struct MsgFormat {
    void (*init)(struct MsgFormat*, uint16_t bufsize);
    uint16_t bufsize; 
    uint16_t (*do_agg)(struct MsgFormat*,uint8_t participantid, munit*, byte*);
    char (*getParicipantMask)(struct MsgFormat*, byte*);
    uint16_t (*getTotalLength)(struct MsgFormat*, byte*);
} MsgFormat;

#endif