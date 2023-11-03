#include "src/temp_sensor.h"
#include "src/debug.h"

/*---------------------------------------------------------------------------*/
PROCESS(main_process, "main_process");
PROCESS(meas_process, "temp sensor process");
AUTOSTART_PROCESSES(&main_process, &meas_process);
/*---------------------------------------------------------------------------*/

PROCESS_THREAD(main_process, ev, data)
{
  PROCESS_BEGIN();

  while(1) {
    printf("Hello, world\n");
    PROCESS_YIELD();
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
