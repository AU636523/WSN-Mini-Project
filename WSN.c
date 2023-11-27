#include "contiki.h"
#include "contiki-net.h"
#include "sys/node-id.h"

#include "project-conf.h"

#include "circularContainer.h"
#include "randSensor.h"
#include "nullMeasurer.h"
#include "simpleCommunication.h"
#include "net/ipv6/uip-ds6-nbr.h"

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
struct EnergyMeasurer energyMeasurer;
struct Communication communication;
struct MsgFormat msgformat;

/* Timers */
static struct etimer main_loop_timer, measurement_timer;

/********** Main Process **********/
PROCESS_THREAD(main_process, ev, data)
{
  PROCESS_BEGIN();

  /*** Set node ID ***/
  node_id = NETWORKING_ID;
      
  /*** Init Components ***/
  randSensor_init(&sensor);
  nullMeasurer_init(&energyMeasurer);
  simpleCommunication_init(&communication, &communication_process);
  deltaConcatFormat_init(&msgformat, UIP_CONF_BUFFER_SIZE);
  circularContainer_init(&mcontainer);

  /*** Init Periodic Timer ***/
  etimer_set(&main_loop_timer, CLOCK_SECOND * 1);
  etimer_set(&measurement_timer, CLOCK_SECOND * 1);

  /*** Wait for DAG ***/
  while(!communication.amIReachable(&communication)) {
    /* Process wait till in DAG */
    LOG_INFO("Waiting for routing\n");
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&main_loop_timer));
    /* Wait and try again */
    etimer_reset(&main_loop_timer);
  } LOG_INFO("Found DAG\n");

  /*** Main Loop ***/
  while(1) {    
    /* Nothing to do in the main loop for now */
    while(0);

    /* Wait */
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&main_loop_timer));
    etimer_reset(&main_loop_timer);
  }

  PROCESS_END();
}

/********** Measurement Process **********/
PROCESS_THREAD(measurement_process, ev, data)
{
  PROCESS_BEGIN();
  LOG_INFO("Measurement Process Started\n");

  /*** Main Loop ***/
  while(1) 
  {
    /* Get Measurement */
    munit u = sensor.getMeasurement(&sensor);

    /* Save Measurement to container */
    mcontainer.put(&mcontainer, u);

    LOG_INFO("Alright, I got a measurement: %d\n", u);

    if(mcontainer._inIdx >= SEND_BUFFER_SIZE) {process_poll(&communication_process); LOG_INFO("Got %d measurements - Polling Communication Process\n", mcontainer._inIdx); }

    /* Wait till timer expired */
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&measurement_timer));
    etimer_reset(&measurement_timer);
  }

  PROCESS_END();
}

/********** Communication Process **********/
PROCESS_THREAD(communication_process, ev, data)
{
  PROCESS_BEGIN();
  LOG_INFO("Communication Process Started\n");

  while(1) {

    /* Wait till we have data to send and are polled by other measurement process */
    PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_POLL);

    /* Add own data to message */
    LOG_INFO("Adding own data to message\n");
    uint16_t newMsgLength = msgformat.do_agg(&msgformat,NETWORKING_ID,mcontainer.getBatch(&mcontainer), communication._buffer);

    /* Get ip of immideate parent */
    uip_ipaddr_t parent_ip;
    NETSTACK_ROUTING.ext_header_srh_get_next_hop(&parent_ip);

    /* Send message to parent */
    simpleCommunication_send(&communication, &parent_ip, communication._buffer, newMsgLength);

    /* Log info */
    LOG_INFO("Send message with participant id %d to ", communication._buffer[DELTACONCATFORMAT_PARTICIPANT_MASK_LOC]); LOG_INFO_6ADDR(&parent_ip); LOG_INFO_(" with %d bytes\n", newMsgLength);
    
    /* Clean buffer */
    communication._buffer[0] = 0x00;
  }
  
  PROCESS_END();
}