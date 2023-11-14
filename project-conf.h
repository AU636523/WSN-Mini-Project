#define RPL_CONF_WITH_STORING           0
#define RPL_CONF_WITH_NON_STORING       0
#define RPL_CONF_MOP RPL_MOP_STORING_MULTICAST

#define IPV6_PREFIX_0 0x2
#define IPV6_PREFIX_1 0x2
#define IPV6_PREFIX_2 0x2

#define UDP_ROOT_ID 0x1

#ifndef NETWORK_ID
    #define NETWORK_ID 1
#endif

#define UDP_PORT 123
#define OUTBUF_SIZE 64
#define MAX_NBRS 8