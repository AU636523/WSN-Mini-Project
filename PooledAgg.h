#ifndef MINMAXAGG_H
#define MINMAXAGG_H

#include "MsgFormat.h"

/********** Pooled Agg Format **********/
/*
Format: 
    * Header: 
        * 2 bytes: Participant mask
    * Data Across participant: (8 bytes)
        * Value Size bytes: Min Value (2 bytes)
        * Value Size bytes: Max Value (2 bytes)
        * Value Size bytes: Avg Value (2 bytes)
*/

#define POOLEDAGG_VALUE_SIZE 2
#define POOLEDAGG_HEADER_SIZE 2

#define POOLEDAGG_PARTICIPANT_MASK_LOC 0
#define POOLEDAGG_MIN_VALUE_LOC POOLEDAGG_PARTICIPANT_MASK_LOC + POOLEDAGG_HEADER_SIZE
#define POOLEDAGG_MAX_VALUE_LOC POOLEDAGG_MIN_VALUE_LOC + POOLEDAGG_VALUE_SIZE
#define POOLEDAGG_AVG_VALUE_LOC POOLEDAGG_MAX_VALUE_LOC + POOLEDAGG_VALUE_SIZE

void pooledAgg_init(MsgFormat* this);
uint16_t pooledAgg_agg(MsgFormat* this, byte* currentBuffer, byte* incommingMsg, uint16_t msglen);
uint16_t pooledAgg_aggOwnValues(MsgFormat* this, munit* own, byte* buffer);

#endif