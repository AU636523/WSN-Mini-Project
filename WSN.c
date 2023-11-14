#include "contiki.h"
#include "project-conf.h"
#include "src/NetDriver/udp.h"

/*---------------------------------------------------------------------------*/
PROCESS(main_process, "main_process");
AUTOSTART_PROCESSES(&main_process);
/*---------------------------------------------------------------------------*/

/* Global Variables */
uint8_t counter; 

UDP_Driver_t netdrv; 
static struct etimer send_timer; //Periodic timer

PROCESS_THREAD(main_process, ev, data)
{
  PROCESS_BEGIN();
  UDP_Driver_init(&netdrv, NETWORK_ID);
  
  etimer_set(&send_timer, CLOCK_SECOND * 2);

  while(1) {

    if (!netdrv.drv.amIinNetwork(&netdrv))
    {
      LOG_INFO("Havent found my DAG yet\n");
    }

    for(counter = 0; counter < UIP_UDP_CONNS; counter++)
    {
      netdrv.drv.sendMsg(&netdrv, counter, netdrv.outbuf, 
        snprintf(netdrv.outbuf, sizeof(netdrv.outbuf), "Hello from %u", netdrv.drv.ID) );
    }
      
    
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&send_timer));
    etimer_reset(&send_timer);
  }

  PROCESS_END();
}
