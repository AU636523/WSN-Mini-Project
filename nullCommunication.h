#ifndef NULLCOMMUNICATION_H
#define NULLCOMMUNICATION_H

#include "Communication.h"

void nullCommunication_init(Communication *c, void *recv_cb);
int nullCommunication_send(struct Communication *c, byte* msg, int len); 
int nullCommunication_incomming_msgs_waiting(struct Communication*);

#endif
