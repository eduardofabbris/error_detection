/*******************************************************************************
* File Name: WD_reset.h
* Version 1.10
*
* Description:
*  This file provides constants and parameter values for the WD_reset
*  component.
*
********************************************************************************
* Copyright 2016, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(WD_reset_CY_MCWDT_PDL_H)
#define WD_reset_CY_MCWDT_PDL_H

#if defined(__cplusplus)
extern "C" {
#endif

#include "cyfitter.h"
#include "mcwdt/cy_mcwdt.h"

/*******************************************************************************
*   Variables
*******************************************************************************/
/**
* \addtogroup group_globals
* @{
*/
extern uint8_t  WD_reset_initVar;
extern const cy_stc_mcwdt_config_t WD_reset_config;
/** @} group_globals */

/***************************************
*   Conditional Compilation Parameters
****************************************/
#define WD_reset_C0_CLEAR_ON_MATCH  (0U)
#define WD_reset_C1_CLEAR_ON_MATCH  (0U)
#define WD_reset_CASCADE_C0C1       (0U)
#define WD_reset_CASCADE_C1C2       (0U)
#define WD_reset_C0_MATCH           (1U)
#define WD_reset_C0_MODE            (2U)
#define WD_reset_C1_MATCH           (32768U)
#define WD_reset_C1_MODE            (0U)
#define WD_reset_C2_PERIOD          (16U)
#define WD_reset_C2_MODE            (0U)

#if (0u == 1U)
    #define WD_reset_CTR0_EN_MASK   0UL
#else
    #define WD_reset_CTR0_EN_MASK   CY_MCWDT_CTR0
#endif
#if (0u == 0U)
    #define WD_reset_CTR1_EN_MASK   0UL
#else
    #define WD_reset_CTR1_EN_MASK   CY_MCWDT_CTR1
#endif
#if (0u == 0U)
    #define WD_reset_CTR2_EN_MASK   0UL
#else
    #define WD_reset_CTR2_EN_MASK   CY_MCWDT_CTR2
#endif

#define WD_reset_ENABLED_CTRS_MASK  (WD_reset_CTR0_EN_MASK |\
                                             WD_reset_CTR1_EN_MASK |\
                                             WD_reset_CTR2_EN_MASK)
											 
#if (1U == WD_reset_C0_MODE) || (3U == WD_reset_C0_MODE)
    #define WD_reset_CTR0_INT_MASK   CY_MCWDT_CTR0
#else
    #define WD_reset_CTR0_INT_MASK   0UL
#endif
#if (1U == WD_reset_C1_MODE) || (3U == WD_reset_C1_MODE)
    #define WD_reset_CTR1_INT_MASK   CY_MCWDT_CTR1
#else
    #define WD_reset_CTR1_INT_MASK   0UL
#endif
#if (1U == WD_reset_C2_MODE)
    #define WD_reset_CTR2_INT_MASK   CY_MCWDT_CTR2
#else
    #define WD_reset_CTR2_INT_MASK   0UL
#endif 

#define WD_reset_CTRS_INT_MASK      (WD_reset_CTR0_INT_MASK |\
                                             WD_reset_CTR1_INT_MASK |\
                                             WD_reset_CTR2_INT_MASK)										 

/***************************************
*        Registers Constants
***************************************/

/* This is a ptr to the base address of the MCWDT instance. */
#define WD_reset_HW                 (WD_reset_MCWDT__HW)

#if (0u == WD_reset_MCWDT__IDX)
    #define WD_reset_RESET_REASON   CY_SYSLIB_RESET_SWWDT0
#else
    #define WD_reset_RESET_REASON   CY_SYSLIB_RESET_SWWDT1
#endif 

#define WD_reset_TWO_LF_CLK_CYCLES_DELAY (62u)


/*******************************************************************************
*        Function Prototypes
*******************************************************************************/
/**
* \addtogroup group_general
* @{
*/
                void     WD_reset_Start(void);
                void     WD_reset_Stop(void);
__STATIC_INLINE cy_en_mcwdt_status_t WD_reset_Init(const cy_stc_mcwdt_config_t *config);
__STATIC_INLINE void     WD_reset_DeInit(void);
__STATIC_INLINE void     WD_reset_Enable(uint32_t counters, uint16_t waitUs);
__STATIC_INLINE void     WD_reset_Disable(uint32_t counters, uint16_t waitUs);
__STATIC_INLINE uint32_t WD_reset_GetEnabledStatus(cy_en_mcwdtctr_t counter);
__STATIC_INLINE void     WD_reset_Lock(void);
__STATIC_INLINE void     WD_reset_Unlock(void);
__STATIC_INLINE uint32_t WD_reset_GetLockedStatus(void);
__STATIC_INLINE void     WD_reset_SetMode(cy_en_mcwdtctr_t counter, cy_en_mcwdtmode_t mode);
__STATIC_INLINE cy_en_mcwdtmode_t WD_reset_GetMode(cy_en_mcwdtctr_t counter);
__STATIC_INLINE void     WD_reset_SetClearOnMatch(cy_en_mcwdtctr_t counter, uint32_t enable);
__STATIC_INLINE uint32_t WD_reset_GetClearOnMatch(cy_en_mcwdtctr_t counter);
__STATIC_INLINE void     WD_reset_SetCascade(cy_en_mcwdtcascade_t cascade);
__STATIC_INLINE cy_en_mcwdtcascade_t WD_reset_GetCascade(void);
__STATIC_INLINE void     WD_reset_SetMatch(cy_en_mcwdtctr_t counter, uint32_t match, uint16_t waitUs);
__STATIC_INLINE uint32_t WD_reset_GetMatch(cy_en_mcwdtctr_t counter);
__STATIC_INLINE void     WD_reset_SetToggleBit(uint32_t bit);
__STATIC_INLINE uint32_t WD_reset_GetToggleBit(void);
__STATIC_INLINE uint32_t WD_reset_GetCount(cy_en_mcwdtctr_t counter);
__STATIC_INLINE void     WD_reset_ResetCounters(uint32_t counters, uint16_t waitUs);
__STATIC_INLINE uint32_t WD_reset_GetInterruptStatus(void);
__STATIC_INLINE void     WD_reset_ClearInterrupt(uint32_t counters);
__STATIC_INLINE void     WD_reset_SetInterrupt(uint32_t counters);
__STATIC_INLINE uint32_t WD_reset_GetInterruptMask(void);
__STATIC_INLINE void     WD_reset_SetInterruptMask(uint32_t counters);
__STATIC_INLINE uint32_t WD_reset_GetInterruptStatusMasked(void);
__STATIC_INLINE uint32_t WD_reset_GetCountCascaded(void);
/** @} general */


/*******************************************************************************
* Function Name: WD_reset_Init
****************************************************************************//**
*
* Invokes the Cy_MCWDT_Init() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE cy_en_mcwdt_status_t WD_reset_Init(const cy_stc_mcwdt_config_t *config)
{
    return (Cy_MCWDT_Init(WD_reset_HW, config));
}


/*******************************************************************************
* Function Name: WD_reset_DeInit
****************************************************************************//**
*
* Invokes the Cy_MCWDT_DeInit() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void WD_reset_DeInit(void)
{
    Cy_MCWDT_DeInit(WD_reset_HW);
}


/*******************************************************************************
* Function Name: WD_reset_Enable
****************************************************************************//**
*
* Invokes the Cy_MCWDT_Enable() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void WD_reset_Enable(uint32_t counters, uint16_t waitUs)
{
    Cy_MCWDT_Enable(WD_reset_HW, counters, waitUs);
}


/*******************************************************************************
* Function Name: WD_reset_Disable
****************************************************************************//**
*
* Invokes the Cy_MCWDT_Disable() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void WD_reset_Disable(uint32_t counters, uint16_t waitUs)
{
    Cy_MCWDT_Disable(WD_reset_HW, counters, waitUs);
}


/*******************************************************************************
* Function Name: WD_reset_GetEnabledStatus
****************************************************************************//**
*
* Invokes the Cy_MCWDT_GetEnabledStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t WD_reset_GetEnabledStatus(cy_en_mcwdtctr_t counter)
{
    return (Cy_MCWDT_GetEnabledStatus(WD_reset_HW, counter));
}


/*******************************************************************************
* Function Name: WD_reset_Lock
****************************************************************************//**
*
* Invokes the Cy_MCWDT_Lock() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void WD_reset_Lock(void)
{
    Cy_MCWDT_Lock(WD_reset_HW);
}


/*******************************************************************************
* Function Name: WD_reset_Unlock
****************************************************************************//**
*
* Invokes the Cy_MCWDT_Unlock() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void WD_reset_Unlock(void)
{
   Cy_MCWDT_Unlock(WD_reset_HW);
}


/*******************************************************************************
* Function Name: WD_reset_GetLockStatus
****************************************************************************//**
*
* Invokes the Cy_MCWDT_GetLockedStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t WD_reset_GetLockedStatus(void)
{
    return (Cy_MCWDT_GetLockedStatus(WD_reset_HW));
}


/*******************************************************************************
* Function Name: WD_reset_SetMode
****************************************************************************//**
*
* Invokes the Cy_MCWDT_SetMode() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void WD_reset_SetMode(cy_en_mcwdtctr_t counter, cy_en_mcwdtmode_t mode)
{
    Cy_MCWDT_SetMode(WD_reset_HW, counter, mode);
}


/*******************************************************************************
* Function Name: WD_reset_GetMode
****************************************************************************//**
*
* Invokes the Cy_MCWDT_GetMode() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE cy_en_mcwdtmode_t WD_reset_GetMode(cy_en_mcwdtctr_t counter)
{
    return (Cy_MCWDT_GetMode(WD_reset_HW, counter));
}


/*******************************************************************************
* Function Name: WD_reset_SetClearOnMatch
****************************************************************************//**
*
* Invokes the Cy_MCWDT_SetClearOnMatch() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void WD_reset_SetClearOnMatch(cy_en_mcwdtctr_t counter, uint32_t enable)
{
    Cy_MCWDT_SetClearOnMatch(WD_reset_HW, counter, enable);
}


/*******************************************************************************
* Function Name: WD_reset_GetClearOnMatch
****************************************************************************//**
*
* Invokes the Cy_MCWDT_GetClearOnMatch() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t WD_reset_GetClearOnMatch(cy_en_mcwdtctr_t counter)
{
    return (Cy_MCWDT_GetClearOnMatch(WD_reset_HW, counter));
}


/*******************************************************************************
* Function Name: WD_reset_SetCascade
****************************************************************************//**
*
* Invokes the Cy_MCWDT_SetCascade() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void WD_reset_SetCascade(cy_en_mcwdtcascade_t cascade)
{
    Cy_MCWDT_SetCascade(WD_reset_HW, cascade);
}


/*******************************************************************************
* Function Name: WD_reset_GetCascade
****************************************************************************//**
*
* Invokes the Cy_MCWDT_GetCascade() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE cy_en_mcwdtcascade_t WD_reset_GetCascade(void)
{
    return (Cy_MCWDT_GetCascade(WD_reset_HW));
}


/*******************************************************************************
* Function Name: WD_reset_SetMatch
****************************************************************************//**
*
* Invokes the Cy_MCWDT_SetMatch() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void WD_reset_SetMatch(cy_en_mcwdtctr_t counter, uint32_t match, uint16_t waitUs)
{
    Cy_MCWDT_SetMatch(WD_reset_HW, counter, match, waitUs);
}


/*******************************************************************************
* Function Name: WD_reset_GetMatch
****************************************************************************//**
*
* Invokes the Cy_MCWDT_GetMatch() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t WD_reset_GetMatch(cy_en_mcwdtctr_t counter)
{
    return (Cy_MCWDT_GetMatch(WD_reset_HW, counter));
}


/*******************************************************************************
* Function Name: WD_reset_SetToggleBit
****************************************************************************//**
*
* Invokes the Cy_MCWDT_SetToggleBit() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void WD_reset_SetToggleBit(uint32_t bit)
{
    Cy_MCWDT_SetToggleBit(WD_reset_HW, bit);
}

/*******************************************************************************
* Function Name: WD_reset_GetToggleBit
****************************************************************************//**
*
* Invokes the Cy_MCWDT_GetToggleBit() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t WD_reset_GetToggleBit(void)
{
    return (Cy_MCWDT_GetToggleBit(WD_reset_HW));
}


/*******************************************************************************
* Function Name: WD_reset_GetCount
****************************************************************************//**
*
* Invokes the Cy_MCWDT_GetCount() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t WD_reset_GetCount(cy_en_mcwdtctr_t counter)
{
    return (Cy_MCWDT_GetCount(WD_reset_HW, counter));
}


/*******************************************************************************
* Function Name: WD_reset_ResetCounters
****************************************************************************//**
*
* Invokes the Cy_MCWDT_ResetCounters() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void WD_reset_ResetCounters(uint32_t counters, uint16_t waitUs)
{
    Cy_MCWDT_ResetCounters(WD_reset_HW, counters, waitUs);
}


/*******************************************************************************
* Function Name: WD_reset_GetInterruptStatus
****************************************************************************//**
*
* Invokes the Cy_MCWDT_GetInterruptStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t WD_reset_GetInterruptStatus(void)
{
    return (Cy_MCWDT_GetInterruptStatus(WD_reset_HW));
}


/*******************************************************************************
* Function Name: WD_reset_ClearInterrupt
****************************************************************************//**
*
* Invokes the Cy_MCWDT_ClearInterrupt() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void WD_reset_ClearInterrupt(uint32_t counters)
{
    Cy_MCWDT_ClearInterrupt(WD_reset_HW, counters);
}


/*******************************************************************************
* Function Name: WD_reset_SetInterrupt
****************************************************************************//**
*
* Invokes the Cy_MCWDT_SetInterrupt() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void WD_reset_SetInterrupt(uint32_t counters)
{
    Cy_MCWDT_SetInterrupt(WD_reset_HW, counters);
}


/*******************************************************************************
* Function Name: WD_reset_GetInterruptMask
****************************************************************************//**
*
* Invokes the Cy_MCWDT_GetInterruptMask() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t WD_reset_GetInterruptMask(void)
{
    return (Cy_MCWDT_GetInterruptMask(WD_reset_HW));
}


/*******************************************************************************
* Function Name: WD_reset_SetInterruptMask
****************************************************************************//**
*
* Invokes the Cy_MCWDT_SetInterruptMask() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void WD_reset_SetInterruptMask(uint32_t counters)
{
    Cy_MCWDT_SetInterruptMask(WD_reset_HW, counters);
}


/*******************************************************************************
* Function Name: WD_reset_GetInterruptStatusMasked
****************************************************************************//**
* Invokes the Cy_MCWDT_GetInterruptStatusMasked() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t WD_reset_GetInterruptStatusMasked(void)
{
    return (Cy_MCWDT_GetInterruptStatusMasked(WD_reset_HW));
}


/*******************************************************************************
* Function Name: WD_reset_GetCountCascaded
****************************************************************************//**
* Invokes the Cy_MCWDT_GetCountCascaded() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t WD_reset_GetCountCascaded(void)
{
    return (Cy_MCWDT_GetCountCascaded(WD_reset_HW));
}

#if defined(__cplusplus)
}
#endif

#endif /* WD_reset_CY_MCWDT_PDL_H */


/* [] END OF FILE */
