#include "contiki.h"
#include <stdio.h> 
//Temp readings
#include "temp_sensor.h"
#include "ring_buffer.h"
#include "dev/sensor/sht11/sht11-sensor.h"

/*---------------------------------------------------------------------------*/
PROCESS(main_process, "main_process");
PROCESS(meas_process, "temp sensor process");
AUTOSTART_PROCESSES(&main_process, &meas_process);
/*---------------------------------------------------------------------------*/

PROCESS_THREAD(main_process, ev, data)
{
  PROCESS_BEGIN();
  process_post(&meas_process, ev, data);

  while(1) {
    printf("Hello, world\n");
    PROCESS_YIELD();
  }

  PROCESS_END();
}

/********************************/
/***** Measurements Process *****/
/********************************/

//Private Variables
static ring_buffer_t mbuf; //Storage of temperatures as raw UINT16 values (see temp_sensor.h for macro for conversion)
static struct etimer measurement_timer;

PROCESS_THREAD(meas_process, ev, data)
{
  PROCESS_BEGIN();
  printf("Measurement Process Started\n");
  SENSORS_ACTIVATE(sht11_sensor);
  
  //Initialize timer
  etimer_set(&measurement_timer, CLOCK_SECOND);

  while(1) {
    if(!ring_buffer_full(&mbuf))
    {
      uint16_t temperature_raw = sht11_sensor.value(SHT11_SENSOR_TEMP);
      ring_buffer_write(&mbuf, temperature_raw);
      printf("Temp %d written to FIFO buffer..\n",temperature_raw);
    }
    else
    {
      printf("Buffer full..\n");
    }

    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&measurement_timer));
    etimer_reset(&measurement_timer);
  }
  PROCESS_END();
}
