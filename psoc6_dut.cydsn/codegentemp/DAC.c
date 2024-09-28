/*******************************************************************************
* File Name: DAC.c
* Version 2.0
*
* Description:
*  This file provides the source code to the API for the DAC
*  component.
*
* Note:
*  None.
*
********************************************************************************
* Copyright 2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/
#include "DAC.h"
#include <syslib/cy_syslib.h>

uint8_t DAC_initVar = 0u;

const cy_stc_ctdac_config_t DAC_ctdacConfig =
{
    .refSource          = DAC_CTDAC_REFSOURCE,
    .formatMode         = DAC_CTDAC_SIGNMODE,
    .updateMode         = DAC_CTDAC_UPDATEMODE,
    .deglitchMode       = DAC_CTDAC_DEGLITCHMODE,
    .outputMode         = CY_CTDAC_OUTPUT_VALUE,
    .outputBuffer       = DAC_CTDAC_OUTPUTBUFFER,
    .deepSleep          = DAC_CTDAC_DEEPSLEEP,
    .deglitchCycles     = DAC_DEGLITCH_TARGET_CYCLES,
    .value              = DAC_PARAM_InitialCode,
    .nextValue          = DAC_PARAM_InitialCode,
    .enableInterrupt    = true,
    .configClock        = false, /* Creator and the schematic will manage the clock */
    .dividerType        = CY_SYSCLK_DIV_8_BIT,
    .dividerNum         = CY_CTDAC_DEINIT,
    .dividerIntValue    = CY_CTDAC_DEINIT,
    .dividerFracValue   = CY_CTDAC_DEINIT,
};

#if DAC_PARAM_OutputBufferUsed
    const cy_stc_ctb_opamp_config_t DAC_outputBufferConfig =
    {
        .deepSleep         = DAC_CTB_VOUTBUF_DEEPSLEEP,
        .oaPower           = DAC_CTB_VOUTBUF_POWER,
        .oaMode            = DAC_CTB_VOUTBUF_DRIVE,
        .oaPump            = DAC_CTB_VOUTBUF_PUMP,
        .oaCompEdge        = CY_CTB_COMP_EDGE_DISABLE,
        .oaCompLevel       = CY_CTB_COMP_DSI_TRIGGER_OUT_PULSE,
        .oaCompBypass      = CY_CTB_COMP_BYPASS_SYNC,
        .oaCompHyst        = CY_CTB_COMP_HYST_DISABLE,
        .oaCompIntrEn      = false,
    };
#endif /* DAC_PARAM_OutputBufferUsed */

#if DAC_PARAM_ReferenceBufferUsed
    const cy_stc_ctb_opamp_config_t DAC_refBufferConfig =
    {
        .deepSleep         = DAC_CTB_REFBUF_DEEPSLEEP,
        .oaPower           = DAC_CTB_REFBUF_POWER,
        .oaMode            = DAC_CTB_REFBUF_DRIVE,
        .oaPump            = DAC_CTB_REFBUF_PUMP,
        .oaCompEdge        = CY_CTB_COMP_EDGE_DISABLE,
        .oaCompLevel       = CY_CTB_COMP_DSI_TRIGGER_OUT_PULSE,
        .oaCompBypass      = CY_CTB_COMP_BYPASS_SYNC,
        .oaCompHyst        = CY_CTB_COMP_HYST_DISABLE,
        .oaCompIntrEn      = false,
    };
#endif /* DAC_PARAM_ReferenceBufferUsed */

/** The instance-specific context structure.
* It is used in the DAC_DeepSleepCallbackParams for managing the deglitch mode.
* The user should not modify anything in this structure.
*/
static cy_stc_ctdac_context_t DAC_ctdacContext;

static cy_stc_syspm_callback_params_t DAC_DeepSleepCallbackParams = {
    .base = DAC_CTDAC_HW,
    .context = (void *) &DAC_ctdacContext,
};

cy_stc_syspm_callback_t DAC_DeepSleepCallbackStruct = {
    .callback = &Cy_CTDAC_DeepSleepCallback,
    .type = CY_SYSPM_DEEPSLEEP,
    .callbackParams = &DAC_DeepSleepCallbackParams,
};

/*******************************************************************************
* Function Name: DAC_Start
****************************************************************************//**
*
*  Initializes the DAC with default customizer
*  values when called the first time (after reset) and enables the DAC.
*  For subsequent calls the configuration is left unchanged and the DAC is
*  just enabled.
*
*  DAC_initVar: this global variable is used to indicate the initial
*  configuration of this component. The variable is initialized to zero and set
*  to 1 the first time DAC_Start() is called. This allows
*  enabling/disabling a component without re-initialization in all subsequent
*  calls to the DAC_Start() routine.
*
*******************************************************************************/
void DAC_Start(void)
{
    if(0u == DAC_initVar)
    {
       DAC_Init();
       DAC_initVar = 1u;

        /* Redirect the AREF IPTAT to the CTB IZTAT when the 100 nA current range is selected.
         * The CTB IPTAT will be High-Z. */
    #if ((DAC_PARAM_ReferenceBufferUsed || DAC_PARAM_OutputBufferUsed) \
        && DAC_COMP_AREF_CURRENT_LOW_USED)
        Cy_CTB_EnableRedirect();
    #endif
    }

    /* Enable the component */
    DAC_Enable();
}

/*******************************************************************************
* Function Name: DAC_SetDeepSleepMode
****************************************************************************//**
*
* Sets whether the DAC hardware continues to stay powered in
* Deep Sleep mode or not. If the DAC is enabled in Deep Sleep,
* then any required buffers will also be enabled in Deep Sleep.
* If the DAC is disabled in Deep Sleep, and both opamps are
* used by the DAC then the opamps will also be disabled.
*
* This function is only useful when the \ref customizer_enable_deepsleep
* checkbox was enabled in the customizer. If the checkbox is disabled,
* the analog router may not use routes capable of Deep Sleep mode so
* Deep Sleep functionality is not guaranteed.
*
* \param deepSleepMode
* - CY_CTDAC_DEEPSLEEP_ENABLE: Enable operation in Deep Sleep mode
* - CY_CTDAC_DEEPSLEEP_DISABLE: Disable operation in Deep Sleep mode
*
*******************************************************************************/
void DAC_SetDeepSleepMode(cy_en_ctdac_deep_sleep_t deepSleepMode)
{
#if (DAC_PARAM_DeepSleep != DAC_COMP_DEEPSLEEP_OFF)

    switch(deepSleepMode)
    {
    case CY_CTDAC_DEEPSLEEP_DISABLE:
        Cy_CTDAC_SetDeepSleepMode(DAC_CTDAC_HW, CY_CTDAC_DEEPSLEEP_DISABLE);

        /* Disable the CTB for Deep Sleep only if both opamps are used */
    #if (DAC_PARAM_ReferenceBufferUsed && DAC_PARAM_OutputBufferUsed)
        Cy_CTB_SetDeepSleepMode(DAC_CTB_HW, CY_CTB_DEEPSLEEP_DISABLE);
    #endif
        break;
    case CY_CTDAC_DEEPSLEEP_ENABLE:
    default:
        Cy_CTDAC_SetDeepSleepMode(DAC_CTDAC_HW, CY_CTDAC_DEEPSLEEP_ENABLE);

        /* Enable the CTB for Deep Sleep if any opamps are used */
    #if (DAC_PARAM_ReferenceBufferUsed || DAC_PARAM_OutputBufferUsed)
        Cy_CTB_SetDeepSleepMode(DAC_CTB_HW, CY_CTB_DEEPSLEEP_ENABLE);
    #endif

        break;
    }

#endif /* (DAC_PARAM_DeepSleep != DAC_COMP_DEEPSLEEP_OFF) */
}

/*******************************************************************************
* Function Name: DAC_SetSampleAndHold
****************************************************************************//**
*
* Performs one of three sample and hold modes.
*
*   - To sample the DAC output, call with \ref DAC_SH_SAMPLE.
*     It takes 10 us for the voltage to be sampled on the S/H capacitor.
*   - To hold the DAC output before entering Deep Sleep, call with \ref DAC_SH_HOLD.
*   - To disconnect the S/H capacitor and enable the DAC output, call with
*     \ref DAC_SH_DISABLE.
*
* This function is useful only when the output is buffered
* and the \ref customizer_sample_hold checkbox is checked in the customizer.
*
* See DAC_EasySampleAndHold() for a complete sequence of
* function calls needed to sample and hold the DAC output.
*
* <b>Note</b> to hold the DAC output in Deep Sleep mode, the DAC and the output buffer opamp
* must be enabled for Deep Sleep operation. The \ref customizer_enable_deepsleep
* checkbox in the customizer should be enabled during build time to ensure
* the opamp and the routing will be available in Deep Sleep mode.
*
* <b>Note</b> the sample and hold switches are enabled by the DAC hardware.
* The DAC hardware should remaine enabled during any sample and hold modes.
*
* \param mode
* - \ref DAC_SH_SAMPLE : Sample the DAC output
* - \ref DAC_SH_HOLD : Hold the DAC output
* - \ref DAC_SH_DISABLE : Disconnect the S/H capacitor
*
*******************************************************************************/
void DAC_SetSampleAndHold(DAC_en_sample_hold_t mode)
{
#if (DAC_PARAM_OutputBufferUsed && (DAC_PARAM_SampleAndHold > 0uL))
    switch(mode)
    {
    case DAC_SH_SAMPLE:
        Cy_CTB_DACSampleAndHold(DAC_CTB_HW, CY_CTB_SH_PREPARE_SAMPLE);

        /* Enable DAC output */
        Cy_CTDAC_SetOutputMode(DAC_CTDAC_HW, CY_CTDAC_OUTPUT_VALUE);

        Cy_CTB_DACSampleAndHold(DAC_CTB_HW, CY_CTB_SH_SAMPLE);
        break;
    case DAC_SH_HOLD:
        Cy_CTB_DACSampleAndHold(DAC_CTB_HW, CY_CTB_SH_PREPARE_HOLD);

        /* Disable DAC output */
        Cy_CTDAC_SetOutputMode(DAC_CTDAC_HW, CY_CTDAC_OUTPUT_HIGHZ);

        Cy_CTB_DACSampleAndHold(DAC_CTB_HW, CY_CTB_SH_HOLD);
        break;
    case DAC_SH_DISABLE:
    default:
        Cy_CTB_DACSampleAndHold(DAC_CTB_HW, CY_CTB_SH_DISABLE);

        /* Enable DAC output */
        Cy_CTDAC_SetOutputMode(DAC_CTDAC_HW, CY_CTDAC_OUTPUT_VALUE);
        break;
    }
#endif /* (DAC_PARAM_OutputBufferUsed && (DAC_PARAM_SampleAndHold > 0uL)) */
}

/*******************************************************************************
* Function Name: DAC_EasySampleAndHold
****************************************************************************//**
*
* Performs entire sequence of API functions to sample and hold the
* DAC output.
*
* -# If DAC hardware is not enabled,
*    -# Enable DAC hardware
*    -# Wait 2 us for DAC hardware to start up
* -# Perform sample by calling DAC_SetSampleAndHold() with DAC_SH_SAMPLE
* -# Wait 10 us to allow for hold cap to charge
* -# Perform hold by calling DAC_SetSampleAndHold() with DAC_SH_HOLD
*
* This function is useful only when the output is buffered
* and the sample and hold feature is enabled in the customizer.
*
* This function should be called periodically to refresh the voltage on the hold capacitor.
* The hold time depends on the supply and reference voltages.
* The following hold times are based on the time it takes
* for the opamp output to change by 1 LSB.
*
*   - Hold time = 750 us @ Vref = VDDA , VDDA = 1.7 V
*   - Hold time = 525 us @ Vref = VDDA , VDDA = 3.6 V
*   - Hold time = 200 us @ Vref = 1.2 V, VDDA = 3.6 V
*
* To hold the DAC output in Deep Sleep mode, the DAC and the output buffer opamp
* must be enabled for Deep Sleep operation. The \ref customizer_enable_deepsleep
* checkbox in the customizer should be enabled during build time to ensure
* the opamp and the routing will be available in Deep Sleep mode.
*
* This function does not manage the VDAC reference buffer opamp, if used.
* This is left to the user.
* The reference opamp can be powered down in Deep Sleep for additional
* power savings. If powered down, the wake up time of the opamp will need
* to be considered. The opamp wakeup time can range from 1 us (without
* charge pump) to 25 us (with charge pump). By default, the reference buffer opamp
* is configured for Medium power.
*
* <b>Note</b> the opamps use reference currents from the Analog Reference (AREF) block.
* Refer to the TRM for the AREF startup times.
*
* <b>Note</b> the sample and hold switches are enabled by the DAC hardware.
* The DAC hardware should remain enabled during any sample and hold modes.
*
*******************************************************************************/
void DAC_EasySampleAndHold(void)
{
#if (DAC_PARAM_OutputBufferUsed && (DAC_PARAM_SampleAndHold > 0uL))
    if (CTDAC_CTDAC_CTRL_ENABLED_Msk != (DAC_CTDAC_HW->CTDAC_CTRL & CTDAC_CTDAC_CTRL_ENABLED_Msk))
    {
        Cy_CTDAC_Enable(DAC_CTDAC_HW);
        Cy_SysLib_DelayUs(2);
    }

    DAC_SetSampleAndHold(DAC_SH_SAMPLE);

    Cy_SysLib_DelayUs(10);

    DAC_SetSampleAndHold(DAC_SH_HOLD);
#endif /* (DAC_PARAM_OutputBufferUsed && (DAC_PARAM_SampleAndHold > 0uL)) */
}
