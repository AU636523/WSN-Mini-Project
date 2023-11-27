#include <stdbool.h>
#include <stdint.h>

#define DEBUG_LEVEL_RPL DEBUG_LEVEL_DBG

/* Networking Defines */
#define UDP_PORT 123
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

#define CHECK_ALL_BITS_ARE_SAT_FROM_MASK(mask, value) ((mask & value) == mask)