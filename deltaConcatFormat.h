#ifndef DELTACONCATFORMAT_H
#define DELTACONCATFORMAT_H

#include "MsgFormat.h"

// Across participants //
#define DELTACONCATFORMAT_PARTICIPANT_MASK_LOC 0
#define DELTACONCATFORMAT_PARTICIPANT_MASK_SIZE 1


// Per participant //
#define DELTACONCATFORMAT_PARTICIPANT_ID_LOC 1

// Participant base value
#define DELTACONCATFORMAT_PARTICIPANT_ID_SIZE 1
#define DELTACONCATFORMAT_PARTICIPANT_BASEVALUE_SIZE 2
#define DELTACONCATFORMAT_PARTICIPANT_BASEVALUE_LOC \
    DELTACONCATFORMAT_PARTICIPANT_ID_LOC + DELTACONCATFORMAT_PARTICIPANT_ID_SIZE

// Participant delta values
#define DELTACONCATFORMAT_PARTICIPANT_DELTAVALUE_SIZE 2
#define DELTACONCATFORMAT_DELTAVALUE_COUNT 16
#define DELTACONCATFORMAT_PARTICIPANT_DELTAVALUES_SIZE \
    (DELTACONCATFORMAT_PARTICIPANT_DELTAVALUE_SIZE * DELTACONCATFORMAT_DELTAVALUE_COUNT)

// Total size
#define DELTACONCATFORMAT_PARTICIPANT_TOTAL_SIZE \
    (DELTACONCATFORMAT_PARTICIPANT_ID_SIZE + DELTACONCATFORMAT_PARTICIPANT_BASEVALUE_SIZE + DELTACONCATFORMAT_PARTICIPANT_DELTAVALUES_SIZE)

// Participant delta value start byte array idx
#define DELTACONCATFORMAT_PARTICIPANT_BUFFER_WITH_ID_LOC(i) \
(DELTACONCATFORMAT_PARTICIPANT_MASK_SIZE + (i * DELTACONCATFORMAT_PARTICIPANT_TOTAL_SIZE))

/*
    MsgHeader: 1 bytes
        1 byte: ParticipantMask
    MsgParticipants: 3 + 32 byte = 35 byte
        1 byte: ParticipantID
        2 byte: BaseValue
        16 x 2 byte: DeltaValues
*/

//Public functions
void deltaConcatFormat_init(MsgFormat* m, uint16_t bufsize);
uint16_t deltaConcatFormat_do_agg(MsgFormat* m, uint8_t participantId, munit* d, byte* buf);
uint16_t deltaConcatFormat_getTotalLength(MsgFormat* m, byte* buf);

//Private functions
uint16_t getMsgEnd(byte* buf);
int countBits(unsigned int n);

#endif