/* ###################################################################
**     Filename    : main.c
**     Processor   : S32K1xx
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
** @version 01.00
** @brief
**         Main module.
**         This module contains user's application code.
*/         
/*!
**  @addtogroup main_module main module documentation
**  @{
*/         
/* MODULE main */


/* Including necessary module. Cpu.h contains other modules needed for compiling.*/
#include "Cpu.h"
#include "string.h"
#ifndef RTOS_H
#define RTOS_H

#include "FreeRTOS.h"
#include "task.h"

#endif
//void rxCallback(void *driverState, uart_event_t event, void *userData);

void rtos_start(void);
void receive_data(void *pvParameters);
void send_data(void *pvParameters);

#define PEX_RTOS_START rtos_start
#define BUFFER_SIZE 256U

volatile int exit_code = 0;
char *string = "just do it!";

uint32_t bytesRemaining;
uint8_t buffer[BUFFER_SIZE];
uint8_t bufferIdx;
status_t status;

/* User includes (#include below this line is not maintained by Processor Expert) */

void rxCallback(void *driverState, uart_event_t event, void *userData)
{
    /* Unused parameters */
    (void)driverState;
    (void)userData;

    /* Check the event type */
    if (event == UART_EVENT_RX_FULL)
    {
        /* The reception stops when newline is received or the buffer is full */
        if ((buffer[bufferIdx] != '\n') && (bufferIdx != (BUFFER_SIZE - 2U)))
        {
            /* Update the buffer index and the rx buffer */
            bufferIdx++;
            LPUART_DRV_SetRxBuffer(INST_LPUART1, &buffer[bufferIdx], 1U);
        }
    }
}

void receive_data(void *pvParameters)
{
	while(1)
	{
//	if(LPUART_DRV_GetReceiveStatus(INST_LPUART1, &bytesRemaining) != STATUS_BUSY) {
//				status = LPUART_DRV_GetReceiveStatus(INST_LPUART1, &bytesRemaining);
//				if(status == STATUS_SUCCESS) {
//					/* Append string terminator to the received data */
//					bufferIdx++;
//					buffer[bufferIdx] = 0U;
//					LPUART_DRV_SendDataBlocking(INST_LPUART1, buffer, bufferIdx, 10);
//					/* Reset the buffer index to start a new reception */
//					bufferIdx = 0U;
//					LPUART_DRV_ReceiveData(INST_LPUART1, buffer, 1U);
//				} else {
//					//error handle
//					LPUART_DRV_ReceiveData(INST_LPUART1, buffer, 1U);
//				}
//	    	}

		//PINS_DRV_SetPins(PTD15,1);
		vTaskDelay(1000);
	}
}

void send_data(void *pvParameters)
{
	while(1)
	{
    PINS_DRV_SetPins(PTD,(1<<15));
	vTaskDelay(1000);
	}
}
void rtos_start(void)
{
    UBaseType_t priority = 0U;
    /* Start the two tasks as described in the comments at the top of this
       file. */

    xTaskCreate(send_data, "send_data", configMINIMAL_STACK_SIZE, NULL, ++priority, NULL);
    xTaskCreate(receive_data, "receive_data", configMINIMAL_STACK_SIZE, NULL, ++priority, NULL);

    /* Start the tasks and timer running. */
    vTaskStartScheduler();

    /* If all is well, the scheduler will now be running, and the following line
       will never be reached.  If the following line does execute, then there was
       insufficient FreeRTOS heap memory available for the idle and/or timer tasks
       to be created.  See the memory management section on the FreeRTOS web site
       for more details. */
    for( ;; )
    {
        /* no code here */
    }
}


/*! 
  \brief The main function for the project.
  \details The startup initialization sequence is the following:
 * - startup asm routine
 * - main()
*/
int main(void)
{
  /* Write your local variable definition here */

  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  #ifdef PEX_RTOS_INIT
    PEX_RTOS_INIT();                   /* Initialization of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of Processor Expert internal initialization.                    ***/

  /* Write your code here */
  /* For example: for(;;) { } */
    CLOCK_SYS_Init(g_clockManConfigsArr, CLOCK_MANAGER_CONFIG_CNT,
                      g_clockManCallbacksArr, CLOCK_MANAGER_CALLBACK_CNT);
    CLOCK_SYS_UpdateConfiguration(0U, CLOCK_MANAGER_POLICY_AGREEMENT);

    PINS_DRV_Init(NUM_OF_CONFIGURED_PINS, g_pin_mux_InitConfigArr);

    LPUART_DRV_Init(INST_LPUART1, &lpuart1_State, &lpuart1_InitConfig0);
    LPUART_DRV_InstallRxCallback(INST_LPUART1,rxCallback,NULL);
    LPUART_DRV_ReceiveData(INST_LPUART1,buffer,1);


  /*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;) {
    if(exit_code != 0) {
      break;
    }
  }
  return exit_code;
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/



/* END main */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.1 [05.21]
**     for the NXP S32K series of microcontrollers.
**
** ###################################################################
*/
