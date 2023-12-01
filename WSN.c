#include "contiki.h"
#include "contiki-net.h"

#include "project-conf.h"

#include "randSensor.h"
#include "simpleCommunication.h"
#include "circularContainer.h"
#include "PooledAgg.h"

#include "sys/log.h"
#define LOG_MODULE "WSN"
#define LOG_LEVEL LOG_LEVEL_INFO

/*---------------------------------------------------------------------------*/
PROCESS(main_process, "main_process");
PROCESS(measurement_process, "measurement_process");
PROCESS(communication_process, "communication_process");
AUTOSTART_PROCESSES(&main_process, &measurement_process, &communication_process);
/*---------------------------------------------------------------------------*/

/* Global Components */
struct Sensor sensor;
struct measurementContainer mcontainer;
struct Communication communication;
struct MsgFormat msgformat;

//uint8_t sampleTime;

/* Timers */
static struct etimer main_loop_timer, measurement_timer;

/********** Main Process **********/
PROCESS_THREAD(main_process, ev, data)
{
  PROCESS_BEGIN();
  //sampleTime = 128; //Equals 1 second

  /*** Set node ID ***/
  //node_id = NETWORKING_ID;          //Comment for Cooja, uncomment for real deployment

  /*** Init Components ***/
  randSensor_init(&sensor); //Random Sensor
  pooledAgg_init(&msgformat); //Pooled Aggregator
  simpleCommunication_init(&communication, &communication_process, &msgformat); //Simple UDP Communication
  circularContainer_init(&mcontainer); //Circular Container

  /*** Init Periodic Timer ***/
  etimer_set(&main_loop_timer, CLOCK_SECOND * 30);

  /*** Wait for DAG ***/
  while(!communication.amIReachable(&communication)) {
    /* Process wait till in DAG */
    //LOG_INFO("Waiting for routing\n");
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&main_loop_timer));

    /* Wait and try again */
    etimer_reset(&main_loop_timer);
  } LOG_INFO("Found DAG\n");

  /*** Main Loop ***/
  while(1) {    
    /* Nothing to do in the main loop for now */
    while(0);  
    PROCESS_PAUSE();
    //sampleTime -= 1;
    //LOG_INFO("Sample time: %d (128=1sec)\n", sampleTime); 

    //if (sampleTime == 1) while(1){LOG_INFO("DONE\n"); PROCESS_PAUSE();}

    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&main_loop_timer));
    etimer_reset(&main_loop_timer);
  }

  PROCESS_END();
}

/********** Measurement Process **********/
PROCESS_THREAD(measurement_process, ev, data)
{
  PROCESS_BEGIN();
  //LOG_INFO("Measurement Process Started\n");

  /* Init periodic timer */
  etimer_set(&measurement_timer, CLOCK_SECOND * MEASUREMENT_PERIOD);

  /*** Main Loop ***/
  while(1) 
  {
    /* Get Measurement */
    munit u = sensor.getMeasurement(&sensor);

    /* Save Measurement to container */
    mcontainer.put(&mcontainer, u);

    /* If not node, poll communication process to send if ring buffer filled up */
    if(mcontainer._inIdx >= MESSAGE_MEASUREMENT_COUNT) 
      { process_post(&communication_process,PROCESS_EVENT_MAX,(void*)NULL); }

    /* Ready to do new measurement */
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&measurement_timer));
    etimer_reset(&measurement_timer);
  }

  PROCESS_END();
}

/********** Communication Process **********/
PROCESS_THREAD(communication_process, ev, data)
{
  PROCESS_BEGIN();
  //LOG_INFO("Communication Process Started\n");

  while(1) {
    
    /* Wait till we have data to send and are polled by other measurement process */
    PROCESS_YIELD_UNTIL(ev == PROCESS_EVENT_MAX || ev == PROCESS_EVENT_MSG);

    /* If root, don't send anything */
    if(node_id == RPL_ROOT_ID) { PROCESS_YIELD(); continue; }

    /* Get next route */
    uip_ipaddr_t parent_ip;
    #ifdef AGGREGATION
      bool nextParticipantFound = communication.getNextRouteParticipant(&communication, communication._outBuffer[0], &parent_ip);
      if(!nextParticipantFound) { LOG_INFO("No next participant found\n"); continue; }
    #else
      NETSTACK_ROUTING.get_root_ipaddr(&parent_ip);
    #endif // !AGGREGATION
    
    /* Add our own measurements */
    uint16_t len = communication._msgformat->aggOwn(communication._msgformat, mcontainer.getBatch(&mcontainer),communication._outBuffer);
    
    /* Send */
    communication.send(&communication, &parent_ip, communication._outBuffer, len);    
    
    /* Clean buffer by setting participation mask to zero */
    communication._outBuffer[0] = 0x00;
    LOG_INFO("Cleaned buffer\n");
  }

  PROCESS_END();
}