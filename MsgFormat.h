#ifndef MSGFORMAT_H
#define MSGFORMAT_H

#include "MeasurementContainer.h"

/* Generic Message Header */

#define AGG_GET_VALUE(msg, loc) (msg[loc] << 8) | msg[loc + 1]
#define AGG_SET_VALUE(msg, loc, value) msg[loc] = (value >> 8) & 0xFF; \
    msg[loc + 1] = value & 0xFF;

#define BIT_SET(x,n) ((x) |= (1 << (n)))
#define IS_BIT_SAT(x,n) ((x) & (1 << (n)))

typedef struct MsgFormat {
    void     (*init)            (struct MsgFormat*, uint16_t bufsize); //Init
    uint16_t (*agg)             (struct MsgFormat* this, byte* currentBuffer, byte* incommingMsg, uint16_t msglen); //Aggregate incomming message
    uint16_t (*aggOwn)          (struct MsgFormat* this, munit* own, byte* buffer); //Aggregate own values

} MsgFormat;

uint8_t popcount(uint16_t value);

#endif