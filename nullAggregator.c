#include "nullAggregator.h"
#include "project-conf.h"

void nullAggregator_init(struct DataAggregator *a) {
    a->init = nullAggregator_init;
    a->do_agg = nullAggregator_do_agg;
    return;
}

// Helper function to sort the array
static void sortArray(munit *array, uint8_t size) {
    for (uint8_t i = 0; i < size - 1; i++) {
        for (uint8_t j = 0; j < size - i - 1; j++) {
            if (array[j] > array[j + 1]) {
                munit temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
    }
}
// Data aggregation for series of temperature measurements, without any other series
uint8_t SingleSeriesAggregation(struct DataAggregator *a, munit* in, byte *out){
    // 1. Order the series of measurements.
    // 2. Find the index of the median value in the original series.
    // 3. Set the median value as the baseline by placing it as the first element in the byte array.
    // 4. Place the number of different measurements considered in this series (default is 1) as the last value. 
    //    This will serve as the weight and indicate the number of times averaging was performed on the series.

    // Sort the input sequence 
    sortArray(in, SEND_BUFFER_SIZE);

    // Finding the median
    munit median = in[SEND_BUFFER_SIZE / 2];
    out[0] = (byte)median; // Store median as baseline

    // Storing delta differences
    for (uint8_t i = 0; i < SEND_BUFFER_SIZE; i++) {
        int16_t delta = in[i] - median;
        out[i + 1] = (byte)delta; // Assuming delta fits into a byte, as we use the source value from the sensor, and not the celcius representation. 
    }

    // Number averaging values, as the last value
    out[SEND_BUFFER_SIZE + 2] = 1;

    return SEND_BUFFER_SIZE + 2; // Size is send buffer size + 2 due to baseline and weight value + all the measurements
}

// Data aggregation for multiple series of temperature measurements
uint8_t MultiSeriesAggregation(struct DataAggregator *a, munit* in1, munit* in2, byte *out){
    // 1. Determine the weight of each series, which is the last value in both sequences.
    // 2. Calculate the average baseline using the weights as follows:
    //    a = weight1
    //    b = weight2
    //    Weight for the first series = a / (a + b), weight for the second series = b / (a + b)
    // 3. Compute the average of all values in each series and insert them into the byte array.
    // 4. Update the weight for the sequence by placing the new weight as the final element in the byte array.


    // Sort the input arrays
    sortArray(in1, SEND_BUFFER_SIZE);
    sortArray(in2, SEND_BUFFER_SIZE);

    // Calculate the weighted average baseline
    // find both weights 
    uint8_t weight1 = in1[SEND_BUFFER_SIZE - 1];
    uint8_t weight2 = in2[SEND_BUFFER_SIZE - 1];
    // find both median values 

    // Some confusion with the measurements being uint16, but no more than uint8 should be required
    uint16_t median1 = in1[SEND_BUFFER_SIZE / 2];
    uint16_t median2 = in2[SEND_BUFFER_SIZE / 2];
    // calculate weighted average as baseline 
    uint16_t weightedBaseline = (median1 * weight1 + median2 * weight2) / (weight1 + weight2);
    out[0] = (byte)weightedBaseline;

    // Storing delta differences
    for (uint8_t i = 0; i < SEND_BUFFER_SIZE - 1; i++) { 
        // Restoring the original value
        int8_t delta1 = in1[i] + weight1;
        int8_t delta2 = in2[i] + weight2;
        // Calculate the weighted average based on the previous values 
        uint8_t finalWeight = (delta1 * weight1 + delta2 * weight2) / (weight1 + weight2);
        // Find the weighted average delta value compared to the baseline 
        finalWeight = finalWeight - weightedBaseline;
        // Store and represent the final value as a byte
        out[i + 1] = (byte)finalWeight;
    }

    // Update the weight as the final value in the seuqnce
    out[SEND_BUFFER_SIZE+2] = weight1 + weight2;

    return SEND_BUFFER_SIZE+2;
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
