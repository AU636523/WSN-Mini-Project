#ifndef SIMPLECOMMUNICATION_H
#define SIMPLECOMMUNICATION_H

#include "Communication.h"
#include "net/ipv6/simple-udp.h"
#include "net/ipv6/uip-ds6-nbr.h"

#define SIMPLECOMMUNICATION_RECV_PORT(id) (3000 + id)

/* Message format: 
 * 1 byte header (One Hot Encoded: NetworkID has added measurement)
 * 2 byte total length
 * For each participant:
    * 1 byte nodeID
    * 16 * munit (2 byte) = 32 byte, data
*/

void simpleCommunication_init(Communication *c, struct process* comProcess);
int simpleCommunication_send(struct Communication *c, uip_ipaddr_t* ip, byte* msg, int len);
byte* simpleCommunication_getBuffer(struct Communication* c);
uint16_t simpleCommunication_getPatienceInMs(struct Communication* c);
bool simpleCommunication_amIReachable(struct Communication* c);
bool simpleCommunication_getNextRouteParticipant(struct Communication*, byte currentParticipantMask, uip_ipaddr_t* out);

//Private functions
void setIp(Communication *c, uint8_t id);
uint8_t bitCount(byte b);

#endif
