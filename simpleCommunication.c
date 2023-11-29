#include "simpleCommunication.h"

#include "sys/log.h"

#define LOG_MODULE "simpleCommunication"
#define LOG_LEVEL LOG_LEVEL_INFO

static struct simple_udp_connection udp_in, udp_out;

static struct process* _comProcess;

static struct Communication* _c; //Local reference to communication singleton struct

static void
udp_rx_callback_out(struct simple_udp_connection *c,
         const uip_ipaddr_t *sender_addr,
         uint16_t sender_port,
         const uip_ipaddr_t *receiver_addr,
         uint16_t receiver_port,
         const uint8_t *data,
         uint16_t datalen)
{LOG_INFO("Callback should not be called on udp out\n");}

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
    c->send = simpleCommunication_send;
    c->outgoing_buffer = (byte*)&uip_appdata;
    c->amIReachable = simpleCommunication_amIReachable;
    c->getNextRouteParticipant = simpleCommunication_getNextRouteParticipant;
    _c = c;

    uint8_t rs_in, rs_out;

    rs_in = simple_udp_register(&udp_in, UDP_RECV_PORT, NULL, UDP_SEND_PORT, udp_rx_callback);
    LOG_INFO("Registered UDP in Connection with status %d\n", rs_in);
    LOG_INFO("Listening (in) to port %d\n", udp_in.local_port);    

    /* Register UDP Connection */
    if(node_id != RPL_ROOT_ID)
    {
        rs_out = simple_udp_register(&udp_out, UDP_SEND_PORT, NULL, UDP_RECV_PORT, udp_rx_callback_out);
        LOG_INFO("Registered UDP out Connection with status %d\n", rs_out);
        LOG_INFO("Listening (not actually) (out) to port %d\n", udp_out.local_port);
    }
    
    /* Start DAG if I am root */
    if (node_id == RPL_ROOT_ID) { LOG_INFO("Starting DAG\n"); NETSTACK_ROUTING.root_start(); }

    /* Init RPL Neigbor Module for route and neighbor manipulation */
    LOG_INFO("Init RPL Neighbor Module\n");
    rpl_neighbor_init();
}

bool simpleCommunication_getNextRouteParticipant(struct Communication* c, byte currentParticipantMask, uip_ipaddr_t* out)
{
    /* Go through participants in DAG */
    rpl_nbr_t *nbr;
    for(nbr = nbr_table_head(rpl_neighbors); nbr != NULL; nbr = nbr_table_next(rpl_neighbors, nbr)) {
        /* Get ip address */
        uip_ipaddr_t *ipaddr = rpl_neighbor_get_ipaddr(nbr);
        /* Get node id from IP */
        uint8_t nbrNodeId = ipaddr->u8[15];

        /* Log info who I found */
        LOG_INFO("Found neighbor: "); LOG_INFO_6ADDR(ipaddr); LOG_INFO_(" with node id %d\n", nbrNodeId);

        /* Check if node is in participant mask */
        if (!IS_BIT_SAT(currentParticipantMask, nbrNodeId))
        {
            if(nbrNodeId < node_id)
                {
                    LOG_INFO("Found next route participant: %d\n", nbrNodeId);
                    /* Set ip address */
                    uip_ipaddr_copy(out, ipaddr);
                    return true;
                }
        }
    }
    LOG_INFO("No next route participant found\n");
    return false;
}

int simpleCommunication_send(struct Communication *c, uip_ipaddr_t* ip, byte* msg, int len)
{
    /* Log info */
    LOG_INFO("Sending to "); LOG_INFO_6ADDR(ip); LOG_INFO_(" %d bytes\n", len);

    /* Send message */
    simple_udp_sendto(&udp_out, msg,len,ip);
    LOG_INFO("Sent %d bytes to ", len); LOG_INFO_6ADDR(ip); LOG_INFO_(" port %d\n", udp_out.remote_port);

    return 0; 
}

byte* simpleCommunication_getBuffer(struct Communication* c)
{
    return c->outgoing_buffer;
}

bool simpleCommunication_amIReachable(struct Communication* c)
{
    bool r = false;
    uip_ipaddr_t root_ip;

    /* Check if root ip address has been found */
    if (NETSTACK_ROUTING.get_root_ipaddr(&root_ip)) r = true;
    
    /* Check if node is reachable */
    else if (NETSTACK_ROUTING.node_is_reachable()) r = true;   

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