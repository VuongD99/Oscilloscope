/******************************************************************************
* File Name: main_cm0p.c
*
* Version: 1.10
*
* Description: This file main application code for the CE223726 EmWin Graphics
*				library TFT Display, for the CM0+ processor.  The only action
*				this file performs is to start the CM4 processor.
*
*/
#include "project.h"

int main(void)
{
    /* __enable_irq(); */ /* Enable global interrupts. */
	
    /* Enable CM4.  CY_CORTEX_M4_APPL_ADDR must be updated if CM4 memory layout is changed. */
    Cy_SysEnableCM4(CY_CORTEX_M4_APPL_ADDR); 

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    for(;;)
    {
        /* Place your application code here. */
		Cy_SysPm_DeepSleep(CY_SYSPM_WAIT_FOR_INTERRUPT);
    }
}

/* [] END OF FILE */
