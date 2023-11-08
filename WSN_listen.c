#include "src/temp_sensor.h"
#include "src/debug.h"
#include "src/udp.h"

/*---------------------------------------------------------------------------*/
PROCESS(main_process, "main_process");
PROCESS(meas_process, "temp sensor process");
AUTOSTART_PROCESSES(&main_process, &meas_process);
/*---------------------------------------------------------------------------*/

static char outbuf[32];
static struct etimer send_timer;

PROCESS_THREAD(main_process, ev, data)
{
  PROCESS_BEGIN();
  udp_init();

  while(1) {
  
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&send_timer));
    etimer_reset(&send_timer);
  }

  PROCESS_END();
}

/********************************/
/***** Measurements Process *****/
/********************************/

static struct etimer measurement_timer;

PROCESS_THREAD(meas_process, ev, data)
{

  PROCESS_BEGIN();
  temp_sensor_init(&measurement_timer);


  while(1) {
    //Write value to buffer
    temp_sensor_save_measurement();

    //Wait and yield 
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&measurement_timer));
    etimer_reset(&measurement_timer);
  }
  PROCESS_END();
}
