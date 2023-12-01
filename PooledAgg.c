#include "PooledAgg.h"

#include "sys/log.h"
#define LOG_MODULE "Agg"
#define LOG_LEVEL LOG_LEVEL_INFO

void pooledAgg_init(MsgFormat* this)
{
    this->agg = &pooledAgg_agg;
    this->aggOwn = &pooledAgg_aggOwnValues;
}

uint16_t pooledAgg_aggOwnValues(MsgFormat* this, munit* ownValues , byte* buffer)
{

    /* Temp buffer */
    byte tempBuffer[POOLEDAGG_HEADER_SIZE + 3*POOLEDAGG_VALUE_SIZE];

    /* Set node ID in buffer */
    AGG_SET_VALUE(tempBuffer, POOLEDAGG_PARTICIPANT_MASK_LOC, (1 << node_id));

    /* Get my own min and max value */
    munit myMinValue = ownValues[0];
    munit myMaxValue = ownValues[0];
    munit myAvgValue = 0;
    
    /* Go through my own values and update myMinValue and myMaxValue */
    uint8_t i;
    for(i = 0; i < MESSAGE_MEASUREMENT_COUNT; i++)
    {
        if(ownValues[i] < myMinValue) myMinValue = ownValues[i];
        if(ownValues[i] > myMaxValue) myMaxValue = ownValues[i];
        myAvgValue += ownValues[i];
    }
    myAvgValue = ((float)myAvgValue / (float)MESSAGE_MEASUREMENT_COUNT);

    /* Update min and max in buffer */
    AGG_SET_VALUE(buffer, POOLEDAGG_MIN_VALUE_LOC, myMinValue);
    AGG_SET_VALUE(buffer, POOLEDAGG_MAX_VALUE_LOC, myMaxValue);
    AGG_SET_VALUE(buffer, POOLEDAGG_AVG_VALUE_LOC, (munit)myAvgValue);

     return pooledAgg_agg(this, buffer, tempBuffer, POOLEDAGG_HEADER_SIZE + 3*POOLEDAGG_VALUE_SIZE);
}

uint16_t pooledAgg_agg(MsgFormat* this, byte* currentBuffer, byte* incommingMsg, uint16_t msglen)
{
    /* Get participant mask */
    uint16_t incommingParticipantMask = AGG_GET_VALUE(incommingMsg, POOLEDAGG_PARTICIPANT_MASK_LOC);
    uint16_t currentParticipantMask = AGG_GET_VALUE(currentBuffer, POOLEDAGG_PARTICIPANT_MASK_LOC);
    
    /* Get min/max from buffer */
    munit minValue = AGG_GET_VALUE(currentBuffer, POOLEDAGG_MIN_VALUE_LOC);
    munit maxValue = AGG_GET_VALUE(currentBuffer, POOLEDAGG_MAX_VALUE_LOC);
    float avgValue = AGG_GET_VALUE(currentBuffer, POOLEDAGG_AVG_VALUE_LOC);

    /* Get min/max/avg from incomming message */
    munit incommingMinValue = AGG_GET_VALUE(incommingMsg, POOLEDAGG_MIN_VALUE_LOC);
    munit incommingMaxValue = AGG_GET_VALUE(incommingMsg, POOLEDAGG_MAX_VALUE_LOC);
    float incommingAvgValue = AGG_GET_VALUE(incommingMsg, POOLEDAGG_AVG_VALUE_LOC);


    /* Update min, max in buffer */
    if(incommingMinValue < minValue) AGG_SET_VALUE(currentBuffer, POOLEDAGG_MIN_VALUE_LOC, incommingMinValue);
    if(incommingMaxValue > maxValue) AGG_SET_VALUE(currentBuffer, POOLEDAGG_MAX_VALUE_LOC, incommingMaxValue);

    /* Update avg in buffer */
    uint8_t incommingParticipantCount = popcount(incommingParticipantMask);
    uint8_t currentParticipantCount = popcount(currentParticipantMask);
    avgValue = (munit)((float)incommingAvgValue*(float)incommingParticipantCount + (float)avgValue * (float)currentParticipantCount) / ((float)currentParticipantCount + (float)incommingParticipantCount);
    AGG_SET_VALUE(currentBuffer, POOLEDAGG_AVG_VALUE_LOC, (munit)avgValue);

    /* Update participant mask */
    uint16_t newParticipantMask = currentParticipantMask | incommingParticipantMask;
    AGG_SET_VALUE(currentBuffer, POOLEDAGG_PARTICIPANT_MASK_LOC, newParticipantMask);

    LOG_INFO("Agg from: ");
    uint8_t i;
    for(i = 0; i < 16; i++)
    {
        if(IS_BIT_SAT(newParticipantMask, i))
            LOG_INFO_("%d, ", i+1);
    }  
    LOG_INFO_("\n");
    
    return POOLEDAGG_HEADER_SIZE + 3*POOLEDAGG_VALUE_SIZE;
}
