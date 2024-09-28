/*******************************************************************************
* File Name: Stopwatch.c
* Version 1.0
*
* Description:
*  This file provides the source code to the API for the Stopwatch
*  component
*
********************************************************************************
* Copyright 2016-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "Stopwatch.h"

/** Indicates whether or not the Stopwatch has been initialized. 
*  The variable is initialized to 0 and set to 1 the first time 
*  Stopwatch_Start() is called. This allows the Component to 
*  restart without reinitialization after the first call to 
*  the Stopwatch_Start() routine.
*/
uint8_t Stopwatch_initVar = 0U;

/** The instance-specific configuration structure. This should be used in the 
*  associated Stopwatch_Init() function.
*/ 
cy_stc_tcpwm_counter_config_t const Stopwatch_config =
{
        .period = 4294967295UL,
        .clockPrescaler = 0UL,
        .runMode = 1UL,
        .countDirection = 0UL,
        .compareOrCapture = 2UL,
        .compare0 = 16384UL,
        .compare1 = 16384UL,
        .enableCompareSwap = false,
        .interruptSources = 0UL,
        .captureInputMode = 3UL,
        .captureInput = CY_TCPWM_INPUT_CREATOR,
        .reloadInputMode = 3UL,
        .reloadInput = CY_TCPWM_INPUT_CREATOR,
        .startInputMode = 3UL,
        .startInput = CY_TCPWM_INPUT_CREATOR,
        .stopInputMode = 3UL,
        .stopInput = CY_TCPWM_INPUT_CREATOR,
        .countInputMode = 3UL,
        .countInput = CY_TCPWM_INPUT_CREATOR,
};


/*******************************************************************************
* Function Name: Stopwatch_Start
****************************************************************************//**
*
*  Calls the Stopwatch_Init() when called the first time and enables 
*  the Stopwatch. For subsequent calls the configuration is left 
*  unchanged and the component is just enabled.
*
* \globalvars
*  \ref Stopwatch_initVar
*
*******************************************************************************/
void Stopwatch_Start(void)
{
    if (0U == Stopwatch_initVar)
    {
        (void)Cy_TCPWM_Counter_Init(Stopwatch_HW, Stopwatch_CNT_NUM, &Stopwatch_config); 

        Stopwatch_initVar = 1U;
    }

    Cy_TCPWM_Enable_Multiple(Stopwatch_HW, Stopwatch_CNT_MASK);
    
    #if (Stopwatch_INPUT_DISABLED == 7UL)
        Cy_TCPWM_TriggerStart(Stopwatch_HW, Stopwatch_CNT_MASK);
    #endif /* (Stopwatch_INPUT_DISABLED == 7UL) */    
}


/* [] END OF FILE */
