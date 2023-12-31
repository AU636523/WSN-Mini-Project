#include <stdbool.h>
#include <stdint.h>

/* Debugging */
//#define LOG_CONF_LEVEL_IPV6                        LOG_LEVEL_ERR
//#define LOG_CONF_LEVEL_RPL                           LOG_LEVEL_ERR
//#define LOG_CONF_LEVEL_6LOWPAN                     LOG_LEVEL_ERR
//#define LOG_CONF_LEVEL_TCPIP                       LOG_LEVEL_ERR
//#define LOG_CONF_LEVEL_MAC                         LOG_LEVEL_ERR
//#define LOG_CONF_LEVEL_FRAMER                      LOG_LEVEL_ERR

//#define LOG_CONF_LEVEL_MAC LOG_LEVEL_INFO
//#define LINK_STATS_CONF_PACKET_COUNTERS 1

/* RPL Configurations */

/* Networking Defines */
#define UDP_SEND_PORT 5678
#define UDP_RECV_PORT 8765
#define RPL_ROOT_ID 1

#ifndef NETWORKING_ID
    #define NETWORKING_ID 1
#endif // !NETWORKING_ID

#define MAX_NODES 8
#define PATIENCE (MAX_NODES - 1)

/* Message Defines */
#define MESSAGE_HEADER_SIZE 1
#define MESSAGE_MEASUREMENT_COUNT 16

/* Measurement Defines */
#define MEASUREMENT_PERIOD 1 //Down to the fraction 1/128


typedef uint16_t munit; //Measurement Unit
typedef unsigned char byte; //Byte