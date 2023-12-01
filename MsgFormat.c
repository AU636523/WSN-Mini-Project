#include "MsgFormat.h"

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