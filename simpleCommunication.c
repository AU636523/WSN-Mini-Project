#include "simpleCommunication.h"
#include "sys/log.h"

#define LOG_MODULE "simpleCommunication"
#define LOG_LEVEL LOG_LEVEL_INFO

/* Local variables */
static struct simple_udp_connection udp_in, udp_out; //UDP Connections
static struct process* _comProcess; //Process to post events to
static struct Communication* _c; //Local reference to communication singleton struct

/********** Receiver Callback Functions **********/

static void
udp_rx_callback_out(struct simple_udp_connection *c, const uip_ipaddr_t *sender_addr, uint16_t sender_port,
         const uip_ipaddr_t *receiver_addr, uint16_t receiver_port, const uint8_t *data, uint16_t datalen )
{
}

static void
udp_rx_callback(struct simple_udp_connection *c, const uip_ipaddr_t *sender_addr, uint16_t sender_port,
         const uip_ipaddr_t *receiver_addr, uint16_t receiver_port, const uint8_t *data, uint16_t datalen )
{
    /* Log info that msg is received */
    LOG_INFO("RX (id %d) PM: ", sender_addr->u8[15]);
    uint16_t participantMask = data[0] << 8 | data[1];
    logParticipantMask(participantMask);
    
    /* Aggregate values */
    _c->_msgformat->agg(_c->_msgformat, _c->_outBuffer, (byte*)data, datalen);
}

void simpleCommunication_init(Communication *c, struct process* comProcess, MsgFormat* msgformat)
{
    /* Save local variables */
    _comProcess = comProcess;
    c->send = simpleCommunication_send;
    c->amIReachable = simpleCommunication_amIReachable;
    c->getNextRouteParticipant = simpleCommunication_getNextRouteParticipant;
    _c = c;

    /* Init msg format */
    c->_msgformat = msgformat;

    simple_udp_register(&udp_in, UDP_RECV_PORT, NULL, UDP_SEND_PORT, udp_rx_callback);
    simple_udp_register(&udp_out, UDP_SEND_PORT, NULL, UDP_RECV_PORT, udp_rx_callback_out);
    
    /* Start DAG if I am root */
    if (node_id == RPL_ROOT_ID) { LOG_INFO("Starting DAG\n"); NETSTACK_ROUTING.root_start(); }

    /* Init RPL Neigbor Module for route and neighbor manipulation */
    rpl_neighbor_init();
}

bool simpleCommunication_getNextRouteParticipant(struct Communication* c, byte currentParticipantMask, uip_ipaddr_t* out)
{
    /* 
        This function builds on the principle that the node with the lowest rank, 
        that is not already in the participant mask, is the next route participant.
    */

    uint16_t lowestRank = 0xFFFF;
    rpl_nbr_t *lowestRankNbr = NULL;

    /* Go through participants in DAG to find relevant next route participant*/
    rpl_nbr_t *nbr;
    for(nbr = nbr_table_head(rpl_neighbors); nbr != NULL; nbr = nbr_table_next(rpl_neighbors, nbr)) {
        
        /* Get ip address */
        uip_ipaddr_t *ipaddr = rpl_neighbor_get_ipaddr(nbr);
        
        /* Get node id from IP */
        uint8_t nbrNodeId = ipaddr->u8[15];

        /* Check if node is in participant mask */
        if (IS_BIT_SAT(currentParticipantMask, nbrNodeId)) continue;

        /* Check metric */
        uint16_t rank = rpl_neighbor_rank_via_nbr(nbr);
        if (rank < lowestRank) {
            lowestRank = rank;
            lowestRankNbr = nbr;
        }
    }

    /* Return neighbor if we found one */
    if (lowestRankNbr != NULL) {
        /* Get IP */
        uip_ipaddr_t *ipaddr = rpl_neighbor_get_ipaddr(lowestRankNbr);
        /* Set IP */
        uip_ipaddr_copy(out, ipaddr);
        /* Log info */
        LOG_INFO("Next route participant: "); LOG_INFO_6ADDR(out); LOG_INFO_(", id %d\n", out->u8[15]);
        return true;
    }
    else 
    {
        /* Log info */
        return false; //No next route participant found
    }   
}

int simpleCommunication_send(struct Communication *c, uip_ipaddr_t* ip, byte* msg, int len)
{
    /* Send message */
    simple_udp_sendto(&udp_out, msg,len,ip);
    LOG_INFO("TX (%d) PM ",ip->u8[15]);
    logParticipantMask(msg[0] << 8 | msg[1]);
    return 0; 
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

void logParticipantMask(uint16_t participantMask)
{
    uint8_t i;
    for(i = 0; i < 16; i++)
    {
        if(IS_BIT_SAT(participantMask, i))
            LOG_INFO_("%d, ", i);
    }  
    LOG_INFO_("\n");
}