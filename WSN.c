#include "contiki.h"
#include <stdio.h> 

/*---------------------------------------------------------------------------*/
PROCESS(main_process, "main_process");
AUTOSTART_PROCESSES(&main_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(main_process, ev, data)
{
  PROCESS_BEGIN();

  while(1) {
    printf("Hello, world\n");
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
