#include "circularContainer.h"

#include "sys/log.h"
#define LOG_MODULE "circularContainer"
#define LOG_LEVEL LOG_LEVEL_INFO


void circularContainer_init(measurementContainer* c)
{
    c->_inIdx = 0;
    c->getBatch = circularContainer_getBatch;
    c->put = circularContainer_put;
}
int circularContainer_put(measurementContainer* c, munit in)
{
    //LOG_INFO("Put: %d\n", in);
    c->container[c->_inIdx % SEND_BUFFER_SIZE] = in;
    //LOG_INFO("Put: %d\n", c->container[c->_inIdx % SEND_BUFFER_SIZE]);
    c->_inIdx++;
    LOG_INFO("Idx: %d\n", c->_inIdx);
    
    return 1;
}
munit* circularContainer_getBatch(measurementContainer* c)
{
    c->_inIdx = 0;
    return c->container;
}
