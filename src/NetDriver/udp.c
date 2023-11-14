
#include "src/NetDriver/udp.h"
 
static uint8_t counter;

void udp_rx_callback(struct simple_udp_connection *c,
         const uip_ipaddr_t *sender_addr,
         uint16_t sender_port,
         const uip_ipaddr_t *receiver_addr,
         uint16_t receiver_port,
         const uint8_t *msg,
         uint16_t msglen)
{
    LOG_INFO("Received message '%.*s' to ", msglen, (char *) msg);
    LOG_INFO_6ADDR(sender_addr);
    LOG_INFO_("\n");
}

bool UDP_Driver_init(UDP_Driver_t* me, uint8_t myID)
{
    LOG_INFO("Starting UDP Connection with id %d\n", myID);

    /* Create instance of Netdriver */
    me->drv.ID = myID;
    me->drv.parentID = (myID > 1 ? (myID - 1) : 0);
    me->drv.rootID = 1;

    /* Set my IP */
    uip_ipaddr(&me->prefixedIPBuffer, IPV6_PREFIX_0, IPV6_PREFIX_1, IPV6_PREFIX_2, myID);
    uip_ds6_set_addr_iid(&me->prefixedIPBuffer, &uip_lladdr);
    uip_ds6_addr_add(&me->prefixedIPBuffer, 0, ADDR_AUTOCONF);

    /* Start DAG if I am root */
    if (myID == UDP_ROOT_ID) NETSTACK_ROUTING.root_start();

    /* Register Contiki UDP Driver */
    return simple_udp_register(&me->c, UDP_PORT, NULL, UDP_PORT, udp_rx_callback);
}
static bool isIDReachable(UDP_Driver_t* me, uint8_t id)
{
    //Todo
    return false;
}

bool UDP_Driver_isParentLinkAvaliable(UDP_Driver_t* me) {return isIDReachable(me, me->drv.parentID);}

bool UDP_Driver_isRootLinkAvaliable(UDP_Driver_t* me) {return isIDReachable(me, me->drv.rootID);}

bool UDP_Driver_sendMsg(UDP_Driver_t* me, uint8_t destID, char* msg, uint16_t msglen)
{
    uint8_t loc_suc = 0; 

    if( isIDReachable(me,destID) )
    {
        UDP_SET_BUF_IP(me, destID);
        loc_suc = simple_udp_sendto(&me->c, msg, msglen, &me->prefixedIPBuffer);
        
        LOG_INFO("Send message '%.*s' to ", msglen, (char *) msg);
        LOG_INFO_6ADDR(&me->prefixedIPBuffer);
        LOG_INFO_("\n");
    }
    else
    {
        LOG_ERR("%d not avaliable yet", destID);
    }

    return (bool) loc_suc;
}

uint8_t UDP_Driver_getNetworkNodes(UDP_Driver_t* me, uint8_t* out_id_list)
{
    counter = 0; 
    uip_ds6_nbr_t nbr = *uip_ds6_nbr_head();
    out_id_list[counter] = uip_ds6_nbr_get_ipaddr(&nbr)->u8[3];

    for (counter = 1; counter < uip_ds6_nbr_num(); counter++)
    {
        out_id_list[counter] = uip_ds6_nbr_get_ipaddr(uip_ds6_nbr_next(&nbr))->u8[3];
    }

    return counter;
}



bool UDP_Driver_amIinNetwork(UDP_Driver_t* me)
{
    return NETSTACK_ROUTING.node_is_reachable();
}