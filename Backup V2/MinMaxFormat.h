#ifndef MINMAXFORMAT_H
#define MINMAXFORMAT_H

#include "MsgFormat.h"

/********** Helper Functions **********/
uint8_t popcount(uint16_t value);

/********** Min Max Format **********/
/*
Format: 
    * Header: 
        * 2 bytes: Participant mask
    * Data Across participant: (6 bytes)
        * 1 bytes: Value Size (byte) (2 bytes)
        * Value Size bytes: Min Value (2 bytes)
        * Value Size bytes: Max Value (2 bytes)
*/
#define MINMAX_AGG_MIN_VALUE_LOC 2
#define MINMAX_AGG_MAX_VALUE_LOC 2

#define MINMAX_AGG_GET_VALUE(msg, loc) (msg[loc] << 8) | msg[loc + 1]
#define MINMAX_AGG_SET_VALUE(msg, loc, value) msg[loc] = (value >> 8) & 0xFF; msg[loc + 1] = value & 0xFF;

void minMaxAggregator_init(MsgFormat* this);
uint16_t minMaxAggregator_agg(MsgFormat* this, byte* currentBuffer, byte* incommingMsg, uint16_t msglen);
uint16_t minMaxAggregator_aggOwnValues(MsgFormat* this, munit* own, byte* buffer);

#endif