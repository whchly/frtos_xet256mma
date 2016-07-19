#include <hidef.h>      /* common defines and macros */
#include "derivative.h" /* derivative-specific definitions */
#include "Std_Types.h"        
 

void main(void) {
  /* put your own code here */
 

	EnableInterrupts;


  for(;;) {
  watchdog_clr( );
    _FEED_COP(); /* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */
}
