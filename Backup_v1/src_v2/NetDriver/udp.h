#pragma once
#include "NetDriver.h"
#include "net/ipv6/simple-udp.h"
#include "net/ipv6/uip-ds6.h"

#define LOG_MODULE "UDP"
#define LOG_LEVEL LOG_LEVEL_DBG

#define UDP_SET_BUF_IP(m, id) uip_ipaddr(&m->prefixedIPBuffer, IPV6_PREFIX_0, IPV6_PREFIX_1, IPV6_PREFIX_2, id);

typedef struct UDP_Driver
{
    NetDriver_t drv;
    struct simple_udp_connection c;
    uip_ipaddr_t prefixedIPBuffer; 
    void (*rxhandler)(struct simple_udp_connection *c, const uip_ipaddr_t *sender_addr,uint16_t sender_port,
         const uip_ipaddr_t *receiver_addr,uint16_t receiver_port,const uint8_t *data,uint16_t datalen);
    uint8_t* (*getOutputbuffer)(void* me);
    char outbuf[OUTBUF_SIZE];
} UDP_Driver_t;

bool UDP_Driver_init(UDP_Driver_t* me, uint8_t myID);
bool UDP_Driver_isParentLinkAvaliable(UDP_Driver_t* me);
bool UDP_Driver_isRootLinkAvaliable(UDP_Driver_t* me);
bool UDP_Driver_sendMsg(UDP_Driver_t* me, uint8_t destID, char* msg, uint16_t msglen);
bool UDP_Driver_amIinNetwork(UDP_Driver_t* me);