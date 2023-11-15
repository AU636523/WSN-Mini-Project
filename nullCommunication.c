#include "nullCommunication.h"

void nullCommunication_init(struct Communication *c, void *recv_cb) {
    c->send = nullCommunication_send;
    c->incomming_msgs_waiting = nullCommunication_incomming_msgs_waiting;
    return;
}

int nullCommunication_send(struct Communication *c, byte* msg, int len) {
    return 1;
}

int nullCommunication_incomming_msgs_waiting(struct Communication* c)
{
    c->buf[0]++;
    return 0;
}
