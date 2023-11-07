// Including .h files from lecture 4. 
#include "contiki.h"
#include "net/netstack.h"
#include "net/ipv6/simple-udp.h"
#include "net/routing/routing.h"
#include "net/ipv6/uip-debug.h"
#include <stdio.h> /* For printf() */


// Setting local port number for udp both sending and recieving
#define LOCAL_PORT 1234
#define UDP_CONN_SEND_INTERVAL (CLOCK_SECOND * 4)

// Struct to handle INCOMMING messages. 
static struct simple_udp_connection udp_conn;
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
  uip_debug_ipaddr_print(sender_addr);
  printf(" on port %d from port %d with length %d: '%s'\n",
         receiver_port, sender_port, datalen, data);

  // send some form of ack.. 
}

/*---------------------------------------------------------------------------*/
PROCESS(hello_world_process, "Hello world process");
AUTOSTART_PROCESSES(&hello_world_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(hello_world_process, ev, data)
{
  static struct etimer periodic_timer;
  uip_ipaddr_t dest_ipaddr;

  PROCESS_BEGIN();

  simple_udp_register(&udp_conn, LOCAL_PORT, NULL, LOCAL_PORT, udp_rx_callback);

  // For some reason this if statement remowed my errors when compiling.. no idea why.. 
  if (1)
  {
    // Setting the target ip address. 
    uip_ip6addr(&dest_ipaddr, 0xfd00, 0, 0, 0, 0, 0, 0, 1); // Convert the string IP to the uip_ipaddr_t format
    etimer_set(&periodic_timer, UDP_CONN_SEND_INTERVAL);
  }
  
  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));
  
    char str[32];
    snprintf(str, sizeof(str), "Hello, specific mote!");
    simple_udp_sendto(&udp_conn, str, strlen(str), &dest_ipaddr);
    printf("Sent a message to ");
    uip_debug_ipaddr_print(&dest_ipaddr);
    printf("\n");

    etimer_reset(&periodic_timer);
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
