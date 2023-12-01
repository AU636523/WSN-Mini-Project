#define NUM_MEASUREMENTS 16 //Must be to the power of two due to ringbuf.h

#define LOCAL_DEBUG 1
#define LOCAL_PORT 1234

#define UIP_CONF_ND6_SEND_NS 1

#ifdef BUFSIZE
#define UIP_CONF_BUFFER_SIZE BUFSIZE
#endif /* BUFSIZE */