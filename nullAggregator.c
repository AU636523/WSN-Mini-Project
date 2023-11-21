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

