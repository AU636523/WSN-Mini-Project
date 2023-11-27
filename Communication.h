#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include "project-conf.h"
#include "deltaConcatFormat.h"
#include "routing.h"
#include "uip-ds6.h"

typedef struct Communication {
    void (*init)(struct Communication*, struct process* comProcess);
    int (*send)(struct Communication*,  uip_ipaddr_t* ip ,byte* data, int length);
    byte* buf;
    byte* (*getBuf)(struct Communication*);
    uint16_t (*getPatienceInMs)(struct Communication*);
    bool (*amIReachable)(struct Communication*);
    MsgFormat* msgformat;
    byte _buffer[UIP_CONF_BUFFER_SIZE];
} Communication;

#endif