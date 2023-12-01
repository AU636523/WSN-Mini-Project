#ifndef SIMPLECOMMUNICATION_H
#define SIMPLECOMMUNICATION_H

#include "Communication.h"

void simpleCommunication_init(Communication *c, struct process* comProcess, MsgFormat* msgformat);
int simpleCommunication_send(struct Communication *c, uip_ipaddr_t* ip, byte* msg, int len);
bool simpleCommunication_amIReachable(struct Communication* c);
bool simpleCommunication_getNextRouteParticipant(struct Communication*, byte currentParticipantMask, uip_ipaddr_t* out);

//Private functions
void setIp(Communication *c, uint8_t id);
uint8_t bitCount(byte b);
void logParticipantMask(uint16_t participantMask);

#endif
