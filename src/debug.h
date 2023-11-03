#pragma once
#include <stdio.h> 
#include "project-conf.h"

#ifdef LOCAL_DEBUG
    #define DEBUG_MSG(s) {\
     \
    printf(s);  \
     }

    #define DEBUG_MSG_INT(s,d) {\
     \
    printf(s,d);  \
     }
#else
    #define DEBUG_MSG(s) {\
    }
    #define DEBUG_MSG_INT(s,d) {\
    }
#endif