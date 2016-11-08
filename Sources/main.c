/* ###################################################################
**     Filename    : main.c
**     Project     : ServoTest
**     Processor   : MKL25Z128VLK4
**     Version     : Driver 01.01
**     Compiler    : GNU C Compiler
**     Date/Time   : 2016-10-07, 11:18, # CodeGen: 0
**     Abstract    :
**         Main module.
**         This module contains user's application code.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file main.c
** @version 01.01
** @brief
**         Main module.
**         This module contains user's application code.
*/         
/*!
**  @addtogroup main_module main module documentation
**  @{
*/         
/* MODULE main */


/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "SERVO1.h"
#include "Pwm1.h"
#include "PwmLdd1.h"
#include "TU1.h"
#include "LED1.h"
#include "LEDpin1.h"
#include "BitIoLdd1.h"
#include "KSDK1.h"
#include "WAIT1.h"
#include "SERVO2.h"
#include "Pwm2.h"
#include "PwmLdd2.h"
#include "SERVO3.h"
#include "Pwm3.h"
#include "PwmLdd3.h"
#include "SERVO4.h"
#include "Pwm4.h"
#include "PwmLdd4.h"
#include "HF1.h"
#include "CLS1.h"
#include "UTIL1.h"
#include "CS1.h"
#include "AS1.h"
#include "ASerialLdd1.h"
#include "TI1.h"
#include "TimerIntLdd1.h"
#include "TU2.h"
#include "TRG1.h"
#include "TmDt1.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
/* User includes (#include below this line is not maintained by Processor Expert) */
#include "Robo.h"

/*lint -save  -e970 Disable MISRA rule (6.3) checking. */
int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
{
  /* Write your local variable definition here */

  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/

  /* Write your code here */

  LED1_Off();
  Robo_initPos();
  while(1){
	  //Robo_StepForward(10);
	  //Robo_StepTime(1);
	  //WAIT1_Waitms(500);
	  //Robo_initPos();
	  //walk(2,1200,1);
	  //turn(4,1200,1);
	  //walk(2,1200,1);
	  //turn(4,1200,1);
	  //shakeLeg(2,1200,1);
	  updown(1,1200,BIG);
	  //WAIT1_Waitms(3000);
  }

  /*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.5 [05.21]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
