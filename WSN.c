#include "contiki.h"
#include "project-conf.h"
#include "nullAggregator.h"
#include "nullSensor.h"
#include "nullMeasurer.h"
#include "nullCommunication.h"
#include "FIFO.h"

/*---------------------------------------------------------------------------*/
PROCESS(main_process, "main_process");
PROCESS(measurement_process, "measurement_process");
PROCESS(communication_process, "communication_process");
AUTOSTART_PROCESSES(&main_process, &measurement_process, &communication_process);
/*---------------------------------------------------------------------------*/

/* Global Components */
struct Sensor sensor;
struct measurementContainer mcontainer;
struct EnergyMeasurer energyMeasurer;
struct Communication communication;
struct DataAggregator dataAggregator;

/* Timers */
static struct etimer send_timer, measurement_timer;

/***** Main Process *****/
PROCESS_THREAD(main_process, ev, data)
{
  PROCESS_BEGIN();
  
  /*** Init Components ***/
  FIFO_init(&mcontainer);
  nullSensor_init(&sensor);
  nullMeasurer_init(&energyMeasurer);
  nullCommunication_init(&communication, NULL);
  nullAggregator_init(&dataAggregator);
  
  /*** Init Periodic Timer ***/
  etimer_set(&send_timer, CLOCK_SECOND * 32);
  etimer_set(&measurement_timer, CLOCK_SECOND * MEASUREMENT_EVERY_N_SEC);

  /*** Main Loop ***/
  while(1) {

    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&send_timer));
    etimer_reset(&send_timer);
  }

  PROCESS_END();
}

/***** Measurement Process *****/
PROCESS_THREAD(measurement_process, ev, data)
{
  PROCESS_BEGIN();
  LOG_INFO("Measurement Process Started\n");

  /*** Main Loop ***/
  while(1) {
    /* Get Measurement */
    munit u = sensor.getMeasurement(&sensor);
    LOG_INFO("Sensor Get Meas done\n");

    /* Save Measurement to container */
    mcontainer.put(&mcontainer, u);
    LOG_INFO("Put done\n");

    /* Poll Communicatin Process */
    process_post(&communication_process, PROCESS_EVENT_CONTINUE, NULL);
    LOG_INFO("Process post done\n");

    /* Wait till timer expired */
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&measurement_timer));
    etimer_reset(&measurement_timer);
  }

  PROCESS_END();
}

/***** Communication Process *****/
PROCESS_THREAD(communication_process, ev, data)
{
  PROCESS_BEGIN();
  LOG_INFO("Communication Process Started\n");

  /*** Main Loop ***/
  while(1) {
    /* Wait for message from measurement process */
    PROCESS_WAIT_EVENT();
    
    /* If container is critically full, we send a batch */
    if ( mcontainer.noBuffersFull(&mcontainer) >= TOTAL_BUFFERS)
    {
      /* Aggregate Data */
      uint8_t noBytes = dataAggregator.do_agg(&dataAggregator, mcontainer.getBatch(&mcontainer), communication.getBuffer(&communication));
      /* Send Data */
      communication.send(&communication, communication.getBuffer(&communication), noBytes);
    }
  }

  PROCESS_END();
}