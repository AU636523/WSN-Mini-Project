#include "udp_api.h"

static struct simple_udp_connection udp_conn;
static uip_ipaddr_t my_adr;
static uip_ipaddr_t dest_ipaddr;

static void udp_rx_callback(
  struct simple_udp_connection *c,
  const uip_ipaddr_t *sender_addr,
  uint16_t sender_port,
  const uip_ipaddr_t *receiver_addr,
  uint16_t receiver_port,
  const uint8_t *data,
  uint16_t datalen
) {
  // Handle incoming message
  printf("Received message from ");
  // Parsing the address and data..
  //uip_debug_ipaddr_print(sender_addr);
  printf(" on port %d from port %d with length %d: '%s'\n",
         receiver_port, sender_port, datalen, data);

  // send some form of ack.. 
}

void udp_init(int me, int dest)
{
  uip_ip6addr(&my_adr, 0xfd00, 0, 0, 0, 0, 0, 0, me); // Convert My IP
  uip_ip6addr(&dest_ipaddr, 0xfd00, 0, 0, 0, 0, 0, 0, dest); // Convert Destination IP
  simple_udp_register(&udp_conn, LOCAL_PORT, &dest_ipaddr, LOCAL_PORT, udp_rx_callback);
  
  const uip_ipaddr_t *default_prefix = uip_ds6_default_prefix();
  uip_ds6_addr_add(&my_adr, 0, ADDR_AUTOCONF);
}

void udp_msg_send(char *buf, int buflen)
{
    simple_udp_sendto(&udp_conn, buf, sizeof(buf), &dest_ipaddr);
    printf("Sent a message to ");
    //uip_debug_ipaddr_print(&dest_ipaddr);
    printf("\n");
}