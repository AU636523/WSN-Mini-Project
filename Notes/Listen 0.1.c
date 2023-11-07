// Including .h files from lecture 4. 
#include "contiki.h"
#include "net/netstack.h"
#include "net/ipv6/simple-udp.h"
#include "net/routing/routing.h"
#include "net/ipv6/uip-debug.h"
#include <stdio.h> /* For printf() */


// Setting local port number to listen too...
#define LOCAL_PORT 1234
#define START_DELAY (CLOCK_SECOND * 5)

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
    static struct etimer start_timer;

  PROCESS_BEGIN();

    // Set the timer for the delay before listening starts
  etimer_set(&start_timer, START_DELAY);
  PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&start_timer));
  printf("\n\n");
  printf("STARTING...");
  printf("\n");
  printf("NOW LISTENING TO MESSAGES AT PORT: %d\n", LOCAL_PORT);
  // Register UDP connection to the local port.
  // NULL is passed for the remote address, which means it will accept any incoming message on the local port.
  // https://contiki.sourceforge.net/docs/2.6/a01692.html#gac3e4e64babc87e0cca192c55a231dabf 
  simple_udp_register(&udp_conn, LOCAL_PORT, NULL, LOCAL_PORT, udp_rx_callback);
  
  printf("LISTENING...");
  // This process does not need to do anything else, so it could just wait for an event forever.
  // The callback function will take care of handling incoming UDP packets.
  PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_EXIT);

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
