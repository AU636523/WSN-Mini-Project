#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include "project-conf.h"
#include "MsgFormat.h"

#include "net/routing/routing.h"
#include "net/netstack.h"
#include "net/ipv6/simple-udp.h"
#include "net/ipv6/uip-ds6-nbr.h"
#include "net/routing/rpl-lite/rpl-neighbor.h"
#include "node-id.h"

typedef struct Communication {
    //Public
    void    (*init)                     (struct Communication* this, struct process* comProcess);
    int     (*send)                     (struct Communication* this,  uip_ipaddr_t* ip ,byte* data, int length);
    bool    (*amIReachable)             (struct Communication* this);
    bool    (*getNextRouteParticipant)  (struct Communication* this, byte currentParticipantMask, uip_ipaddr_t* out);
    //Private
    MsgFormat* _msgformat;
    byte _outBuffer[UIP_CONF_BUFFER_SIZE - UIP_IPUDPH_LEN];
} Communication;

typedef struct aggregateJob { 
    byte* buffer;
    byte* msg;
    uint16_t msglen;
} aggregateJob;

#endif