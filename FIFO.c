#include "FIFO.h"

void FIFO_init(struct measurementContainer *f)
{
    f->put = FIFO_put;
    f->getBatch = FIFO_getBatch;
    f->_batchPtr = 0;
    return;
}

int FIFO_put(struct measurementContainer *f, munit in) {
    if(f->_batchPtr > TOTAL_BUFFERS*SEND_BUFFER_SIZE) {
        LOG_INFO("Buffers full!\n");
        return 0;
    }
    int buffer = f->_batchPtr / SEND_BUFFER_SIZE;
    int index = f->_batchPtr % SEND_BUFFER_SIZE;
    f->batch[buffer][index] = in;
    f->_batchPtr--;
    return true; 
}

munit* FIFO_getBatch(struct measurementContainer *f) {
    if (f->_batchPtr <= 0) {
        LOG_INFO("Batch not full!\n");
        return NULL;
    }
    munit* b = f->batch[f->_batchPtr / SEND_BUFFER_SIZE];
    f->_batchPtr -= SEND_BUFFER_SIZE;

    return b;
}