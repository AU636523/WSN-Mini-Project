#include "deltaConcatFormat.h"

#include "sys/log.h"

#define LOG_MODULE "MSGFORMAT"
#define LOG_LEVEL LOG_LEVEL_INFO

void deltaConcatFormat_init(MsgFormat* m, uint16_t bufsize)
{
    m->bufsize = bufsize;
    m->do_agg = deltaConcatFormat_do_agg;
    m->getTotalLength = deltaConcatFormat_getTotalLength;
}
uint16_t deltaConcatFormat_do_agg(MsgFormat* m,uint8_t participantId, munit* d, byte* buf)
{
    uint16_t msgEnd = getMsgEnd(buf);
    uint16_t msgLength = msgEnd + DELTACONCATFORMAT_PARTICIPANT_TOTAL_SIZE;

    uint16_t participantBaseValue = d[0];

    /* Check if buffer has space */
    if (msgLength > m->bufsize) { LOG_INFO("UIP Buffer not large enough!"); return 0; }

    /* Check if participant is already in buffer */
    if (BIT_SET(buf[DELTACONCATFORMAT_PARTICIPANT_MASK_LOC], participantId)) { LOG_INFO("Already in buffer, dropping message\n"); return 0; }

    //Set participantID in mask
    buf[msgEnd + DELTACONCATFORMAT_PARTICIPANT_MASK_LOC] |= (1 << participantId);

    //Add basevalue to buf[] over two bytes
    buf[msgEnd + DELTACONCATFORMAT_PARTICIPANT_MASK_SIZE] = participantBaseValue & 0xFF;
    buf[msgEnd + DELTACONCATFORMAT_PARTICIPANT_MASK_SIZE + 1] = participantBaseValue >> 8;

    uint8_t i; 
    for (i = 1; i < DELTACONCATFORMAT_DELTAVALUE_COUNT; i++)
    {
        // Get location of deltaValue
        byte* loc = &buf[msgEnd + DELTACONCATFORMAT_PARTICIPANT_MASK_SIZE + DELTACONCATFORMAT_PARTICIPANT_BASEVALUE_SIZE + (i * DELTACONCATFORMAT_PARTICIPANT_DELTAVALUE_SIZE)];
        // Calculate deltaValue
        uint16_t deltaValue = d[i] - d[i - 1];
        // Split deltavalue over two bytes
        loc[0] = deltaValue & 0xFF;
        loc[1] = deltaValue >> 8;
    }

    return msgLength;
}

uint16_t deltaConcatFormat_getTotalLength(MsgFormat* m, byte* buf)
{
    return countBits(buf[DELTACONCATFORMAT_PARTICIPANT_MASK_LOC]) * DELTACONCATFORMAT_PARTICIPANT_TOTAL_SIZE + DELTACONCATFORMAT_PARTICIPANT_MASK_SIZE;
}

int countBits(unsigned int n) 
{
    int count = 0;
    while (n) {
        count += n & 1;
        n >>= 1;
    }
    return count;
}

uint16_t getMsgEnd(byte* buf)
{
    /* Get count of participants */
    return countBits(buf[DELTACONCATFORMAT_PARTICIPANT_MASK_LOC]);
}