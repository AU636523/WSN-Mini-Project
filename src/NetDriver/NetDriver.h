#pragma once
#include "project-conf.h"

#include "net/routing/routing.h"
#include "net/netstack.h"
#include "../os/sys/log.h"
#include "../os/sys/log-conf.h"
#include <stdint.h>

typedef struct NetDriver {
    bool (*init)(void* me); //Pointer to self
    bool (*isParentLinkAvaliable)(void* me); //Is parent avaliable?
    bool (*isRootLinkAvaliable)(void* me); //Is root link avaliable?
    bool (*amIinNetwork)(void* me);
    bool (*sendMsg)(void* me, uint8_t destID, char* msg, uint16_t msglen); //Send message
    uint8_t (*getIDsInDAG)(void* me, uint8_t* IDOutList);
    uint8_t ID; //My own ID
    uint8_t parentID; //Parent ID
    uint8_t rootID; //Root ID
} NetDriver_t;