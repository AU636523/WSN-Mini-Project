#include "simpleCommunication.h"

#include "sys/log.h"

#define LOG_MODULE "simpleCommunication"
#define LOG_LEVEL LOG_LEVEL_INFO

static struct simple_udp_connection udp;
static struct process* _comProcess;

static struct Communication* _c; //Local reference to communication singleton struct


static void
udp_rx_callback(struct simple_udp_connection *c,
         const uip_ipaddr_t *sender_addr,
         uint16_t sender_port,
         const uip_ipaddr_t *receiver_addr,
         uint16_t receiver_port,
         const uint8_t *data,
         uint16_t datalen)
{
    /* Log info sender, datalen and participant-mask */
    LOG_INFO("Received from "); LOG_INFO_6ADDR(sender_addr); LOG_INFO_(" %d bytes (participant mask: %d)\n", datalen, data[0]);

    /* Get local buffer length */
    uint16_t bufferLength = _c->msgformat->getTotalLength(_c->msgformat, _c->_buffer);
    
    /* Add participants to buffer */
    size_t i; 
    for (i = 0; i < bitCount(data[DELTACONCATFORMAT_PARTICIPANT_MASK_LOC]); i++)
    {
        /* Get participant id */
        byte participantId = data[DELTACONCATFORMAT_PARTICIPANT_MASK_SIZE + (i * DELTACONCATFORMAT_PARTICIPANT_TOTAL_SIZE)];
        
        /* Add data to local buffer */
        const byte* data_in = &data[DELTACONCATFORMAT_PARTICIPANT_MASK_SIZE + (i * DELTACONCATFORMAT_PARTICIPANT_DELTAVALUE_SIZE)];

        /* Check if we have space in buffer */
        if (bufferLength + DELTACONCATFORMAT_PARTICIPANT_TOTAL_SIZE > UIP_CONF_BUFFER_SIZE) { LOG_ERR("Not space in buffer to aggregate\n"); return; }

        /* Copy to local buffer */
        memcpy(&_c->_buffer[bufferLength], data_in, DELTACONCATFORMAT_PARTICIPANT_TOTAL_SIZE);

        /* Set participant id in marticipants mask */
        _c->_buffer[DELTACONCATFORMAT_PARTICIPANT_MASK_LOC] |= (1 << participantId);
        bufferLength += DELTACONCATFORMAT_PARTICIPANT_TOTAL_SIZE;

        /* Log info */
        LOG_INFO("Added participant %d to local buffer\n", participantId);
    }
}

void simpleCommunication_init(Communication *c, struct process* comProcess)
{
    /* Save variables */
    _comProcess = comProcess;
    c->send = &simpleCommunication_send;
    c->buf = (byte*)&uip_appdata;
    c->getPatienceInMs = &simpleCommunication_getPatienceInMs;
    _c = c;

    /* Register UDP Connection */
    simple_udp_register(&udp, UDP_PORT, NULL, UDP_PORT, udp_rx_callback);

    /* Set IP according to node id */
    setIp(c,NETWORKING_ID);
}

int simpleCommunication_send(struct Communication *c, uip_ipaddr_t* ip, byte* msg, int len)
{
    /* Log info */
    LOG_INFO("Sending to "); LOG_INFO_6ADDR(ip); LOG_INFO_(" %d bytes\n", len);

    /* Send message */
    uint16_t s = simple_udp_sendto(&udp, msg, len, ip);

    return s; 
}

void setIp(Communication *c,int id)
{
    /* Configure IP */
    uip_ipaddr_t ipaddr;
    uip_ip6addr(&ipaddr, 0xaaaa, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, id);

    /* Set IP */
    uip_ds6_addr_add(&ipaddr, 0, ADDR_MANUAL);
    LOG_INFO("IP: ");
    LOG_INFO_6ADDR(&ipaddr);
    LOG_INFO_("\n");  
}

uint16_t simpleCommunication_getPatienceInMs(struct Communication* c)
{
    /* Calculate Patience in ms */
    uint16_t patience = (MAX_NODES - NETWORKING_ID) * 1000;

    LOG_INFO("Patience calculated to %d\n", (int)((float)patience/1000.0));

    return patience;
}

byte* simpleCommunication_getBuffer(struct Communication* c)
{
    return c->buf;
}

bool simpleCommunication_amIReachable(struct Communication* c)
{
    bool r = false;
    if (NETSTACK_ROUTING.node_is_reachable()) r = true;
    return r; 
}
uint8_t bitCount(byte b)
{
    uint8_t count = 0;
    while (b)
    {
        count += b & 1;
        b >>= 1;
    }
    return count;
}