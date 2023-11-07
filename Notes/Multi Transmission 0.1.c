// Including .h files from lecture 4. 
#include "contiki.h"
#include "net/netstack.h"
#include "net/ipv6/simple-udp.h"
#include "net/routing/routing.h"
#include "net/ipv6/uip-debug.h"
#include <stdio.h> /* For printf() */


// Setting local port number for udp both sending and recieving
#define LOCAL_PORT 1234
// Interval for resending values
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
  uip_ipaddr_t addr;
  PROCESS_BEGIN();

  // Register the udp connection with a callback for receiving messages. 
  // https://contiki.sourceforge.net/docs/2.6/a01692.html#gac3e4e64babc87e0cca192c55a231dabf 
  simple_udp_register(&udp_conn, LOCAL_PORT, NULL, LOCAL_PORT, udp_rx_callback);

  etimer_set(&periodic_timer, UDP_CONN_SEND_INTERVAL);
  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));

    // Create a link-local all-nodes multicast address
    uip_create_linklocal_allnodes_mcast(&addr); 

    // Prepare the message to be sent
    static char str[32];
    // Creating the buffer for the string.. 
    snprintf(str, sizeof(str), "Hello, multicast message!");

    // transmitting the string
    simple_udp_sendto(&udp_conn, str, strlen(str), &addr);


    printf("Sent multicast message\n");

    etimer_reset(&periodic_timer);
  }
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
