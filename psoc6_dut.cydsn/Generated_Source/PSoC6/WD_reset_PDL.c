/*******************************************************************************
* File Name: WD_reset.c
* Version 1.10
*
* Description:
*  This file provides the source code to the API for the WD_reset
*  component.
*
********************************************************************************
* Copyright 2016, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "WD_reset_PDL.h"


/** WD_reset_initVar indicates whether the WD_reset  component
*  has been initialized. The variable is initialized to 0 and set to 1 the first
*  time WD_reset_Start() is called.
*  This allows the component to restart without reinitialization after the first 
*  call to the WD_reset_Start() routine.
*
*  If re-initialization of the component is required, then the 
*  WD_reset_Init() function can be called before the 
*  WD_reset_Start() or WD_reset_Enable() function.
*/
uint8 WD_reset_initVar = 0u;

/** The instance-specific configuration structure. This should be used in the 
*  associated WD_reset_Init() function.
*/ 
const cy_stc_mcwdt_config_t WD_reset_config =
{
    .c0Match     = WD_reset_C0_MATCH,
    .c1Match     = WD_reset_C1_MATCH,
    .c0Mode      = WD_reset_C0_MODE,
    .c1Mode      = WD_reset_C1_MODE,
    .c2ToggleBit = WD_reset_C2_PERIOD,
    .c2Mode      = WD_reset_C2_MODE,
    .c0ClearOnMatch = (bool)WD_reset_C0_CLEAR_ON_MATCH,
    .c1ClearOnMatch = (bool)WD_reset_C1_CLEAR_ON_MATCH,
    .c0c1Cascade = (bool)WD_reset_CASCADE_C0C1,
    .c1c2Cascade = (bool)WD_reset_CASCADE_C1C2
};


/*******************************************************************************
* Function Name: WD_reset_Start
****************************************************************************//**
*
*  Sets the initVar variable, calls the Init() function, unmasks the 
*  corresponding counter interrupts and then calls the Enable() function 
*  to enable the counters.
*
* \globalvars
*  \ref WD_reset_initVar
*
*  \note
*  When this API is called, the counter starts counting after two lf_clk cycles 
*  pass. It is the user's responsibility to check whether the selected counters
*  were enabled immediately after the function call. This can be done by the 
*  WD_reset_GetEnabledStatus() API.
*
*******************************************************************************/
void WD_reset_Start(void)
{
    if (0u == WD_reset_initVar)
    {
        (void)WD_reset_Init(&WD_reset_config);
        WD_reset_initVar = 1u; /* Component was initialized */
    }

	/* Set interrupt masks for the counters */
	WD_reset_SetInterruptMask(WD_reset_CTRS_INT_MASK);

	/* Enable the counters that are enabled in the customizer */
    WD_reset_Enable(WD_reset_ENABLED_CTRS_MASK, 0u);
}


/*******************************************************************************
* Function Name: WD_reset_Stop
****************************************************************************//**
*
*  Calls the Disable() function to disable all counters.
*
*******************************************************************************/
void WD_reset_Stop(void)
{
    WD_reset_Disable(CY_MCWDT_CTR_Msk, WD_reset_TWO_LF_CLK_CYCLES_DELAY);
    WD_reset_DeInit();
}


/* [] END OF FILE */
