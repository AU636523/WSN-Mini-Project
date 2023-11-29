#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include "project-conf.h"
#include "deltaConcatFormat.h"

#include "net/routing/routing.h"
#include "net/netstack.h"
#include "net/ipv6/simple-udp.h"

#include "net/routing/rpl-lite/rpl-neighbor.h"
#include "node-id.h"

typedef struct Communication {
    void (*init)(struct Communication*, struct process* comProcess);
    int (*send)(struct Communication*,  uip_ipaddr_t* ip ,byte* data, int length);
    bool (*amIReachable)(struct Communication*);
    bool (*getNextRouteParticipant)(struct Communication*, byte currentParticipantMask, uip_ipaddr_t* out);
    MsgFormat* msgformat;
    byte* outgoing_buffer;
    byte _buffer[UIP_CONF_BUFFER_SIZE - UIP_IPUDPH_LEN];
} Communication;

#endif