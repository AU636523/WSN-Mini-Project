#include <stdbool.h>
#include <stdint.h>

/* Debugging */
#define LOG_CONF_LEVEL_IPV6                        LOG_LEVEL_ERR
//#define LOG_CONF_LEVEL_RPL                           LOG_LEVEL_ERR
//#define LOG_CONF_LEVEL_6LOWPAN                     LOG_LEVEL_ERR
//#define LOG_CONF_LEVEL_TCPIP                       LOG_LEVEL_ERR
//#define LOG_CONF_LEVEL_MAC                         LOG_LEVEL_ERR
//#define LOG_CONF_LEVEL_FRAMER                      LOG_LEVEL_ERR

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

/* Measurement Defines */
#define SEND_BUFFER_SIZE 16

typedef uint16_t munit; //Measurement Unit