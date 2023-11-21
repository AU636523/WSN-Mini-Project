#include "nullAggregator.h"
#include "project-conf.h"

void nullAggregator_init(struct DataAggregator *a) {
    a->init = nullAggregator_init;
    a->do_agg = nullAggregator_do_agg;
    return;
}


// Transmitting the mean and variance of the input sequence and stores them in the buffer. 
uint8_t nullAggregator_do_agg(struct DataAggregator *a, munit* in, byte *out) {
    // Calculate deltas
    int deltas[SEND_BUFFER_SIZE - 1];
    for (int i = 0; i < SEND_BUFFER_SIZE - 1; i++) {
        deltas[i] = in[i + 1] - in[i];
    }

    // Calculate mean of deltas
    int mean_delta = 0;
    for (int i = 0; i < SEND_BUFFER_SIZE - 1; i++) {
        mean_delta += deltas[i];
    }
    mean_delta /= (SEND_BUFFER_SIZE - 1);

    // Calculate variance of deltas
    int variance = 0;
    for (int i = 0; i < SEND_BUFFER_SIZE - 1; i++) {
        variance += (deltas[i] - mean_delta) * (deltas[i] - mean_delta);
    }
    variance /= (SEND_BUFFER_SIZE - 1);

    // Store the variance and the mean in the output buffer
    out[0] = (byte)(mean_delta & 0xFF);
    out[1] = (byte)(variance & 0xFF);

    // Return the number of bytes written to the output buffer
    return 2;
}


// Only using the delta variance in the output buffer
/*uint8_t nullAggregator_do_agg(struct DataAggregator *a, munit* in, byte *out) {
    out[0] = (byte)(in[0] & 0xFF);
    int transmitted_bytes = 1;
    // Calculate deltas and store them in a temporary array
    byte deltas[SEND_BUFFER_SIZE - 1];
    for (int i = 0; i < SEND_BUFFER_SIZE - 1; i++) {
        int delta = in[i + 1] - in[i];
        out[transmitted_bytes] = (byte)(delta & 0xFF); 
        transmitted_bytes ++;
    }
    return transmitted_bytes;
}*/

// delta variance using 4 bit representation of the delta values 
/*uint8_t nullAggregator_do_agg(struct DataAggregator *a, munit* in, byte *out) {
    int outIndex = 1; // Index for the output array
    out[0] = (byte)(in[0] & 0xFF);

    // Iterate over the input array in steps of 2 to pack two deltas into one byte
    for (int i = 0; i < SEND_BUFFER_SIZE - 1; i += 2) {
        byte delta1 = (byte)(in[i + 1] - in[i]) & 0x0F; // First 4-bit delta

        byte delta2 = 0;
        if (i + 2 < SEND_BUFFER_SIZE) {
            delta2 = (byte)(in[i + 2] - in[i + 1]) & 0x0F; // Second 4-bit delta
        }

        out[outIndex] = (delta1 << 4) | delta2; // Pack two 4-bit deltas into one byte
        outIndex++;
    }

    return outIndex; // Number of bytes in the output array
}*/
