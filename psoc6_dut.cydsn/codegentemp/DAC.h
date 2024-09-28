/*******************************************************************************
* File Name: DAC.h
* Version 2.0
*
* Description:
*  This file provides constants and parameter values for the DAC
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

#if !defined(CY_DAC_H)
#define CY_DAC_H

#include <cy_device_headers.h>
#include <ctdac/cy_ctdac.h>
#ifdef CTBM0
#include <ctb/cy_ctb.h>
#endif
#include <syspm/cy_syspm.h>

/*******************************************************************************
                            Symbol Data Types
The data types that are used in the customizer are redefined as constants here.
*******************************************************************************/
#define DAC_COMP_REFSOURCE_EXTERNAL            0uL
#define DAC_COMP_REFSOURCE_VDDA                1uL
#define DAC_COMP_REFSOURCE_DWR                 2uL
#define DAC_COMP_FORMAT_UNSIGNED               0uL
#define DAC_COMP_FORMAT_SIGNED                 1uL
#define DAC_COMP_OUTPUT_UNBUFFERED_PIN         0uL
#define DAC_COMP_OUTPUT_UNBUFFERED_INTERNAL    1uL
#define DAC_COMP_OUTPUT_BUFFERED_PIN           2uL
#define DAC_COMP_OUTPUT_BUFFERED_INTERNAL      3uL
#define DAC_COMP_UPDATE_DIRECT                 0uL
#define DAC_COMP_UPDATE_BUFFERED               1uL
#define DAC_COMP_UPDATE_STROBE_EDGE            2uL
#define DAC_COMP_UPDATE_STROBE_IMMEDIATE       3uL
#define DAC_COMP_UPDATE_STROBE_LEVEL           4uL
#define DAC_COMP_DEEPSLEEP_OFF                 0uL
#define DAC_COMP_DEEPSLEEP_ON                  1uL

/*******************************************************************************
                            Internal Constants
*******************************************************************************/
#define DAC_COMP_INTERRUPT_ENABLE          1uL
#define DAC_COMP_INTERRUPT_DISABLE         0uL
#define DAC_COMP_AREF_CURRENT_LOW_USED     (CYDEV_SYSTEM_AREF_CURRENT == CYDEV_SYSTEM_AREF_CURRENT_LOW)

/*******************************************************************************
                                  Parameters
The parameters that are set in the customizer are redefined as constants here.
*******************************************************************************/
#define DAC_PARAM_VrefSource           (1uL)
#define DAC_PARAM_InitialCode          (0)
#define DAC_PARAM_OutputBuffer         (2uL)
#define DAC_PARAM_SampleAndHold        (0uL)
#define DAC_PARAM_OutputBufferPower    (3uL)
#define DAC_PARAM_DacCodeMode          (0uL)
#define DAC_PARAM_UpdateMode           (1uL)
#define DAC_PARAM_DeepSleep            (0uL)
#define DAC_PARAM_ShowTrig             (1uL)
#define DAC_PARAM_OpAmpUsage           (1uL)
#define DAC_PARAM_VrefVoltage_mV       (3300uL)
#define DAC_PARAM_ReferenceBufferUsed  (DAC_PARAM_VrefSource != DAC_COMP_REFSOURCE_VDDA)
#define DAC_PARAM_OutputBufferUsed     (DAC_PARAM_OutputBuffer > DAC_COMP_OUTPUT_UNBUFFERED_INTERNAL)

/*******************************************************************************
                            Enumerated Definitions
*******************************************************************************/
/** This enum type is used when calling DAC_SetSampleAndHold().*/
typedef enum {
    DAC_SH_DISABLE  = 0u,      /**< Disable sample and hold by disconnecting the S/H capacitor */
    DAC_SH_SAMPLE   = 1u,      /**< Sample the VDAC output */
    DAC_SH_HOLD     = 2u,      /**< Hold the VDAC output */
}DAC_en_sample_hold_t;

/*******************************************************************************
                    Variables with External Linkage
*******************************************************************************/

/** Configuration structure for initializing the CTDAC PDL */
extern const cy_stc_ctdac_config_t DAC_ctdacConfig;

#if DAC_PARAM_OutputBufferUsed
    /** Configuration structure for initializing Opamp0 of the CTB as the CTDAC output buffer */
    extern const cy_stc_ctb_opamp_config_t DAC_outputBufferConfig;
#endif

#if DAC_PARAM_ReferenceBufferUsed
    /** Configuration structure for initializing Opamp1 of the CTB as the CTDAC reference buffer */
    extern const cy_stc_ctb_opamp_config_t DAC_refBufferConfig;
#endif

/** Tracks whether block is initialized (1) or not (0). */
extern uint8_t DAC_initVar;

/**
* \addtogroup group_globals
* \{
*/

/** System power management callback structure for Deep Sleep entry and exit.
* This callback is needed in order to disable deglitching before
* the device enters Deep Sleep. This ensures that the deglitch switches
* are closed and that DAC operation in Deep Sleep, if enabled, is correct.
* The callback also ensures that deglitching is re-enabled when the device wakes up.
* Registering this Deep Sleep callback is needed only if Deep Sleep operation
* is enabled in the Customizer Configure dialog (see \ref customizer_enable_deepsleep).
*
* \snippet SUT_2.cydsn/main_cm4.c SNIPPET_DEEPSLEEP
*/
extern cy_stc_syspm_callback_t DAC_DeepSleepCallbackStruct;

/** \} group_globals */

/*******************************************************************************
                        CTDAC Configuration Defines
Constants used in the configuration structure for initializing the CTDAC
*******************************************************************************/
#define DAC_CTDAC_HW                   CTDAC0 /**< The pointer to the base address of the CTDAC instance */

#define DAC_CTDAC_REFSOURCE            ((DAC_PARAM_VrefSource == DAC_COMP_REFSOURCE_VDDA) ? \
                                                    CY_CTDAC_REFSOURCE_VDDA : CY_CTDAC_REFSOURCE_EXTERNAL)
#define DAC_CTDAC_SIGNMODE             ((DAC_PARAM_DacCodeMode == DAC_COMP_FORMAT_SIGNED) ? \
                                                    CY_CTDAC_FORMAT_SIGNED : CY_CTDAC_FORMAT_UNSIGNED)
#define DAC_CTDAC_UPDATEMODE           (cy_en_ctdac_update_t) DAC_PARAM_UpdateMode
#define DAC_CTDAC_OUTPUTBUFFER         ((DAC_PARAM_OutputBuffer <= DAC_COMP_OUTPUT_UNBUFFERED_INTERNAL) ? \
                                                    CY_CTDAC_OUTPUT_UNBUFFERED : CY_CTDAC_OUTPUT_BUFFERED)
#define DAC_CTDAC_DEGLITCHMODE         ((DAC_PARAM_OutputBuffer <= DAC_COMP_OUTPUT_UNBUFFERED_INTERNAL) ? \
                                                    CY_CTDAC_DEGLITCHMODE_UNBUFFERED : CY_CTDAC_DEGLITCHMODE_BUFFERED)
#define DAC_CTDAC_DEEPSLEEP            ((DAC_PARAM_DeepSleep == DAC_COMP_DEEPSLEEP_OFF) ? \
                                                    CY_CTDAC_DEEPSLEEP_DISABLE : CY_CTDAC_DEEPSLEEP_ENABLE)

#define DAC_DEGLITCH_TARGET_TIME_NS    (700uL)
#define DAC_FACTOR_NANO_TO_MICRO       (1000uL)
#define DAC_DEGLITCH_TARGET_CYCLES     (((CYDEV_CLK_PERICLK__MHZ * DAC_DEGLITCH_TARGET_TIME_NS) / DAC_FACTOR_NANO_TO_MICRO) - 1uL)

#if DAC_PARAM_ReferenceBufferUsed
    #define DAC_CTB_HW                     DAC_REFBUF__HW
    #define DAC_CTB_REFBUF_OPAMP_NUM       CY_CTB_OPAMP_1
    #define DAC_CTB_REFBUF_POWER           CY_CTB_POWER_MEDIUM
    #define DAC_CTB_REFBUF_DRIVE           CY_CTB_MODE_OPAMP1X
    #define DAC_CTB_REFBUF_DEEPSLEEP       ((DAC_PARAM_DeepSleep == DAC_COMP_DEEPSLEEP_OFF) ? \
                                                        CY_CTB_DEEPSLEEP_DISABLE : CY_CTB_DEEPSLEEP_ENABLE)
    #if DAC_COMP_AREF_CURRENT_LOW_USED
        #define DAC_CTB_REFBUF_PUMP        CY_CTB_PUMP_DISABLE
    #else
        #define DAC_CTB_REFBUF_PUMP        ((DAC_PARAM_DeepSleep == DAC_COMP_DEEPSLEEP_OFF) ? \
                                                        CY_CTB_PUMP_ENABLE : CY_CTB_PUMP_DISABLE)
    #endif
#endif

/*******************************************************************************
                        CTB Configuration Defines
Constants used in the configuration structure for initializing the CTB
*******************************************************************************/
#if DAC_PARAM_OutputBufferUsed
    #if !defined(DAC_CTB_HW)
        #define DAC_CTB_HW                DAC_VOUTBUF__HW
    #endif
    #define DAC_CTB_VOUTBUF_OPAMP_NUM      CY_CTB_OPAMP_0
    #define DAC_CTB_VOUTBUF_POWER          (cy_en_ctb_power_t) DAC_PARAM_OutputBufferPower
    #define DAC_CTB_VOUTBUF_DRIVE          ((DAC_PARAM_OutputBuffer == DAC_COMP_OUTPUT_BUFFERED_INTERNAL) ? \
                                                            CY_CTB_MODE_OPAMP1X : CY_CTB_MODE_OPAMP10X)
    #define DAC_CTB_VOUTBUF_DEEPSLEEP      ((DAC_PARAM_DeepSleep == DAC_COMP_DEEPSLEEP_OFF) ? \
                                                        CY_CTB_DEEPSLEEP_DISABLE : CY_CTB_DEEPSLEEP_ENABLE)
    #if DAC_COMP_AREF_CURRENT_LOW_USED
        #define DAC_CTB_VOUTBUF_PUMP       CY_CTB_PUMP_DISABLE
    #else
        #define DAC_CTB_VOUTBUF_PUMP       ((DAC_PARAM_DeepSleep == DAC_COMP_DEEPSLEEP_OFF) ? \
                                                        CY_CTB_PUMP_ENABLE : CY_CTB_PUMP_DISABLE)
    #endif
#endif

/*******************************************************************************
                        Function Prototypes
*******************************************************************************/
void DAC_Start(void);
__STATIC_INLINE void DAC_Enable(void);
__STATIC_INLINE void DAC_Init(void);
__STATIC_INLINE void DAC_Stop(void);
__STATIC_INLINE void DAC_SetValue(int32_t value);
__STATIC_INLINE void DAC_SetValueBuffered(int32_t value);
__STATIC_INLINE void DAC_SetSignMode(cy_en_ctdac_format_t signMode);
void DAC_SetDeepSleepMode(cy_en_ctdac_deep_sleep_t deepSleepMode);

void DAC_SetSampleAndHold(DAC_en_sample_hold_t mode);

/**
* \addtogroup group_component
* \{
*/
void DAC_EasySampleAndHold(void);
/** \} group_component */

__STATIC_INLINE void DAC_EnableInterrupt(void);
__STATIC_INLINE void DAC_DisableInterrupt(void);
__STATIC_INLINE uint32_t DAC_GetInterruptMask(void);
__STATIC_INLINE uint32_t DAC_GetInterruptStatus(void);
__STATIC_INLINE uint32_t DAC_GetInterruptStatusMasked(void);
__STATIC_INLINE void DAC_ClearInterrupt(void);
__STATIC_INLINE void DAC_SetInterrupt(void);

/*******************************************************************************
* Function Name: DAC_Enable
****************************************************************************//**
*
* Powers up all required hardware blocks. This includes the DAC hardware block
* and CTB hardware block, if used.
* If no opamps are used, the CTB hardware block is untouched.
*
*******************************************************************************/
__STATIC_INLINE void DAC_Enable(void)
{
    Cy_CTDAC_Enable(DAC_CTDAC_HW);

#if (DAC_PARAM_ReferenceBufferUsed || DAC_PARAM_OutputBufferUsed)
    Cy_CTB_Enable(DAC_CTB_HW);
#endif
}

/*******************************************************************************
* Function Name: DAC_Init
****************************************************************************//**
*
* Initializes all required hardware blocks. This includes the DAC hardware block
* and CTB hardware block, if used.
* If no opamps are used, the CTB hardware block is untouched.
*
* This function should only be called once (after reset) and
* does not enable the hardware.
*
* DAC_Start() is the preferred function to initialize
* and enable the hardware.
*
*******************************************************************************/
__STATIC_INLINE void DAC_Init(void)
{
    (void)Cy_CTDAC_Init(DAC_CTDAC_HW, &DAC_ctdacConfig);

#if DAC_PARAM_ReferenceBufferUsed
    (void)Cy_CTB_OpampInit(DAC_CTB_HW, DAC_CTB_REFBUF_OPAMP_NUM, &DAC_refBufferConfig);
#endif

#if DAC_PARAM_OutputBufferUsed
    (void)Cy_CTB_OpampInit(DAC_CTB_HW, DAC_CTB_VOUTBUF_OPAMP_NUM, &DAC_outputBufferConfig);
#endif
}

/*******************************************************************************
* Function Name: DAC_Stop
****************************************************************************//**
*
* Powers down all required hardware blocks. This includes the DAC hardware block
* and CTB hardware block, if used.
* If no opamps are used, the CTB hardware block is untouched.
*
*******************************************************************************/
__STATIC_INLINE void DAC_Stop(void)
{
    Cy_CTDAC_Disable(DAC_CTDAC_HW);

#if (DAC_PARAM_ReferenceBufferUsed || DAC_PARAM_OutputBufferUsed)
    Cy_CTB_Disable(DAC_CTB_HW);
#endif
}

/*******************************************************************************
* Function Name: DAC_SetValue
****************************************************************************//**
*
* Set the value that the DAC will output on the next Peri clock cycle.
* This function should only be called in the Direct write \ref customizer_update_mode.
*
* Only the least significant 12 bits have an effect.
* Sign extension of negative values is unnecessary and is
* ignored by the hardware. The way in which the DAC interprets the 12-bit
* data is controlled by DAC_SetSignMode().
*
* \param value
* Value to write into the DAC value register.
*
*******************************************************************************/
__STATIC_INLINE void DAC_SetValue(int32_t value)
{
    Cy_CTDAC_SetValue(DAC_CTDAC_HW, value);
}

/*******************************************************************************
* Function Name: DAC_SetValueBuffered
****************************************************************************//**
*
* Set the value that the DAC will output on the next DAC clock cycle or strobe
* event, if strobe is enabled.
* This function should be called in all modes except the Direct write \ref customizer_update_mode.
*
* Only the least significant 12 bits have an effect.
* Sign extension of negative values is unnecessary and is
* ignored by the hardware. The way in which the CTDAC interprets the 12-bit
* data is controlled by DAC_SetSignMode().
*
* \param value
* Value to write into the DAC next value register.
*
*******************************************************************************/
__STATIC_INLINE void DAC_SetValueBuffered(int32_t value)
{
    Cy_CTDAC_SetValueBuffered(DAC_CTDAC_HW, value);
}

/*******************************************************************************
* Function Name: DAC_SetSignMode
****************************************************************************//**
*
* Sets whether to interpret the DAC value as signed or unsigned.
* In unsigned mode, the DAC value register is used without any decoding.
* In signed  mode, the MSB is inverted by adding 0x800 to the DAC value.
* This converts the lowest signed number, 0x800, to the lowest unsigned
* number, 0x000.
*
* \param signMode
* - CY_CTDAC_FORMAT_UNSIGNED: For unsigned mode
* - CY_CTDAC_FORMAT_SIGNED: For signed mode
*
*******************************************************************************/
__STATIC_INLINE void DAC_SetSignMode(cy_en_ctdac_format_t signMode)
{
    Cy_CTDAC_SetSignMode(DAC_CTDAC_HW, signMode);
}

/*******************************************************************************
* Function Name: DAC_EnableInterrupt
****************************************************************************//**
*
* Enable the DAC interrupt to be forwarded to the CPU interrupt
* controller so that it can be handled by a user function.
*
* Interrupts are available in all \ref customizer_update_mode except Direct write.
* The update mode is configured in the GUI basic tab.
*
*******************************************************************************/
__STATIC_INLINE void DAC_EnableInterrupt(void)
{
    Cy_CTDAC_SetInterruptMask(DAC_CTDAC_HW, DAC_COMP_INTERRUPT_ENABLE);
}

/*******************************************************************************
* Function Name: DAC_DisableInterrupt
****************************************************************************//**
*
* Disable the DAC interrupt to be forwarded to the CPU interrupt
* controller.
*
* Interrupts are available in all \ref customizer_update_mode except Direct write.
* The update mode is configured in the GUI basic tab.
*
*******************************************************************************/
__STATIC_INLINE void DAC_DisableInterrupt(void)
{
    Cy_CTDAC_SetInterruptMask(DAC_CTDAC_HW, DAC_COMP_INTERRUPT_DISABLE);
}

/*******************************************************************************
* Function Name: DAC_GetInterruptMask
****************************************************************************//**
*
* Returns whether the DAC interrupt is configured to be
* forwarded to the CPU interrupt controller or not.
* See DAC_EnableInterrupt() and DAC_DisableInterrupt().
*
* Interrupts are available in all \ref customizer_update_mode except Direct write.
* The update mode is configured in the GUI basic tab.
*
* \return
* - 0 = Interrupt is disabled (will not be forwarded to interrupt controller)
* - 1 = Interrupt is enabled (will be forwarded to interrupt controller)
*
*******************************************************************************/
__STATIC_INLINE uint32_t DAC_GetInterruptMask(void)
{
    return Cy_CTDAC_GetInterruptMask(DAC_CTDAC_HW);
}

/*******************************************************************************
* Function Name: DAC_GetInterruptStatus
****************************************************************************//**
*
* Returns the interrupt status which gets set by the hardware
* when the buffered value register, CTDAC_VAL_NXT, is transferred to the
* value register, CTDAC_VAL. Once set, the DAC is ready to accept a new value.
*
* Interrupts are available in all \ref customizer_update_mode except Direct write.
* The update mode is configured in the GUI basic tab.
*
* \return
* - 0 = Value not transferred
* - 1 = Value transferred
*
*******************************************************************************/
__STATIC_INLINE uint32_t DAC_GetInterruptStatus(void)
{
    return Cy_CTDAC_GetInterruptStatus(DAC_CTDAC_HW);
}

/*******************************************************************************
* Function Name: DAC_GetInterruptStatusMasked
****************************************************************************//**
*
* Returns the bitwise AND of DAC_GetInterruptStatus() and
* DAC_GetInterruptMask(). When high, the DAC interrupt is
* asserted and the interrupt is forwarded to the CPU interrupt
* controller.
*
* Interrupts are available in all \ref customizer_update_mode except Direct write.
* The update mode is configured in the GUI basic tab.
*
* \return
* - 0 = Value not transferred or not forwarded to the CPU interrupt controller.
* - 1 = Value transferred and forwarded to the CPU interrupt controller.
*
*******************************************************************************/
__STATIC_INLINE uint32_t DAC_GetInterruptStatusMasked(void)
{
    return Cy_CTDAC_GetInterruptStatusMasked(DAC_CTDAC_HW);
}

/*******************************************************************************
* Function Name: DAC_ClearInterrupt
****************************************************************************//**
*
* Clears the interrupt that was set by the hardware.
* When using the interrupt, it must be cleared with this function so that
* the hardware can set subsequent interrupts and those interrupts
* can be forwarded to the interrupt controller, if enabled.
*
* Interrupts are available in all \ref customizer_update_mode except Direct write.
* The update mode is configured in the GUI basic tab.
*
*******************************************************************************/
__STATIC_INLINE void DAC_ClearInterrupt(void)
{
    Cy_CTDAC_ClearInterrupt(DAC_CTDAC_HW);
}

/*******************************************************************************
* Function Name: DAC_SetInterrupt
****************************************************************************//**
*
* Forces the DAC interrupt to trigger using software.
*
* Interrupts are available in all \ref customizer_update_mode except Direct write.
* The update mode is configured in the GUI basic tab.
*
*******************************************************************************/
__STATIC_INLINE void DAC_SetInterrupt(void)
{
    Cy_CTDAC_SetInterrupt(DAC_CTDAC_HW);
}

#endif /* !defined(CY_DAC_H) */

/* [] END OF FILE */


