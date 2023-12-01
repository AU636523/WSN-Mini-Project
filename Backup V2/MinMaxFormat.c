#include "MinMaxFormat.h"


#include "sys/log.h"
#define LOG_MODULE "Nessage"
#define LOG_LEVEL LOG_LEVEL_INFO

uint16_t minMaxAggregator_aggOwnValues(MsgFormat* this, munit* ownValues , byte* buffer)
{
    /* Set node ID in buffer */
    MINMAX_AGG_SET_VALUE(buffer, MESSAGE_PARTICIPANT_MASK_LOC, (((1 << node_id) )| (MINMAX_AGG_GET_VALUE(buffer, MESSAGE_PARTICIPANT_MASK_LOC))));

    /* Get min/max from buffer */
    munit minValue = MINMAX_AGG_GET_VALUE(buffer, MINMAX_AGG_MIN_VALUE_LOC);
    munit maxValue = MINMAX_AGG_GET_VALUE(buffer, MINMAX_AGG_MAX_VALUE_LOC);

    /* Get my own min and max value */
    munit myMinValue = ownValues[0];
    munit myMaxValue = ownValues[0];
    
    /* Go through my own values and update myMinValue and myMaxValue */
    LOG_INFO("Aggregating own data: ");
    uint8_t i;
    for(i = 0; i < MESSAGE_MEASUREMENT_COUNT; i++)
    {
        if(ownValues[i] < myMinValue) myMinValue = ownValues[i];
        if(ownValues[i] > myMaxValue) myMaxValue = ownValues[i];
        LOG_INFO_("%d, ", ownValues[i]);
    }
    LOG_INFO("\n"); 

    /* Update min and max in buffer */
    if(myMinValue < minValue) MINMAX_AGG_SET_VALUE(buffer, MINMAX_AGG_MIN_VALUE_LOC, myMinValue);
    if(myMaxValue > maxValue) MINMAX_AGG_SET_VALUE(buffer, MINMAX_AGG_MAX_VALUE_LOC, myMaxValue);

    return MESSAGE_HEADER_SIZE + 6;
}

uint16_t minMaxAggregator_agg(MsgFormat* this, byte* currentBuffer, byte* incommingMsg, uint16_t msglen)
{
    LOG_INFO("Aggregating\n");
    /* Get participant mask */
    uint16_t participantMask = MINMAX_AGG_GET_VALUE(incommingMsg, MESSAGE_PARTICIPANT_MASK_LOC);
    
    /* Get min/max from buffer */
    munit minValue = MINMAX_AGG_GET_VALUE(currentBuffer, MINMAX_AGG_MIN_VALUE_LOC);
    munit maxValue = MINMAX_AGG_GET_VALUE(currentBuffer, MINMAX_AGG_MAX_VALUE_LOC);

    /* Get min/max from incomming message */
    munit incommingMinValue = MINMAX_AGG_GET_VALUE(incommingMsg, MINMAX_AGG_MIN_VALUE_LOC);
    munit incommingMaxValue = MINMAX_AGG_GET_VALUE(incommingMsg, MINMAX_AGG_MAX_VALUE_LOC);

    /* Update min and max in buffer */
    if(incommingMinValue < minValue) MINMAX_AGG_SET_VALUE(currentBuffer, MINMAX_AGG_MIN_VALUE_LOC, incommingMinValue);
    if(incommingMaxValue > maxValue) MINMAX_AGG_SET_VALUE(currentBuffer, MINMAX_AGG_MAX_VALUE_LOC, incommingMaxValue);

    /* Update participant mask */
    currentBuffer[MESSAGE_PARTICIPANT_MASK_LOC] |= participantMask;

    LOG_INFO("Current min and max is %d and %d\n", minValue, maxValue);
    uint8_t i;
    for(i = 0; i < 16; i++)
    {
        if(IS_BIT_SAT(participantMask, i)) 
            LOG_INFO("Aggregated data from %d\n", i);
    }    
    LOG_INFO("New min %d and max %d\n", MINMAX_AGG_GET_VALUE(currentBuffer, MINMAX_AGG_MIN_VALUE_LOC), MINMAX_AGG_GET_VALUE(currentBuffer, MINMAX_AGG_MAX_VALUE_LOC));
    
    return MESSAGE_HEADER_SIZE + 6;
}

void minMaxAggregator_init(MsgFormat* this)
{
    this->agg = &minMaxAggregator_agg;
    this->aggOwn = &minMaxAggregator_aggOwnValues;
}

uint8_t popcount(uint16_t value) {
    uint8_t count = 0;
    while (value != 0) {
        if (value & 1) {
            count++;
        }
        value >>= 1;
    }
    return count;
}