#pragma once

#include "contiki.h"
#include "project-conf.h"
#include "contiki-net.h"
#include <stdio.h>

void udp_init(int my_adr, int dest_adr);
void udp_msg_send(char *buf, int buflen);