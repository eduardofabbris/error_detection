/*******************************************************************************
* File Name: Stopwatch.h
* Version 1.0
*
* Description:
*  This file provides constants and parameter values for the Stopwatch
*  component.
*
********************************************************************************
* Copyright 2016-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(Stopwatch_CY_TCPWM_COUNTER_PDL_H)
#define Stopwatch_CY_TCPWM_COUNTER_PDL_H

#include "cyfitter.h"
#include "tcpwm/cy_tcpwm_counter.h"

   
/*******************************************************************************
* Variables
*******************************************************************************/
/**
* \addtogroup group_globals
* @{
*/
extern uint8_t  Stopwatch_initVar;
extern cy_stc_tcpwm_counter_config_t const Stopwatch_config;
/** @} group_globals */


/***************************************
*  Conditional Compilation Parameters
***************************************/

#define Stopwatch_INIT_COMPARE_OR_CAPTURE    (2UL)


/***************************************
*        Function Prototypes
****************************************/
/**
* \addtogroup group_general
* @{
*/
void Stopwatch_Start(void);
__STATIC_INLINE cy_en_tcpwm_status_t Stopwatch_Init(cy_stc_tcpwm_counter_config_t const *config);
__STATIC_INLINE void Stopwatch_DeInit(void);
__STATIC_INLINE void Stopwatch_Enable(void);
__STATIC_INLINE void Stopwatch_Disable(void);
__STATIC_INLINE uint32_t Stopwatch_GetStatus(void);

#if(CY_TCPWM_COUNTER_MODE_CAPTURE == Stopwatch_INIT_COMPARE_OR_CAPTURE)
    __STATIC_INLINE uint32_t Stopwatch_GetCapture(void);
    __STATIC_INLINE uint32_t Stopwatch_GetCaptureBuf(void);
#else
    __STATIC_INLINE void Stopwatch_SetCompare0(uint32_t compare0);
    __STATIC_INLINE uint32_t Stopwatch_GetCompare0(void);
    __STATIC_INLINE void Stopwatch_SetCompare1(uint32_t compare1);
    __STATIC_INLINE uint32_t Stopwatch_GetCompare1(void);
    __STATIC_INLINE void Stopwatch_EnableCompareSwap(bool enable);
#endif /* (CY_TCPWM_COUNTER_MODE_CAPTURE == Stopwatch_INIT_COMPARE_OR_CAPTURE) */

__STATIC_INLINE void Stopwatch_SetCounter(uint32_t count);
__STATIC_INLINE uint32_t Stopwatch_GetCounter(void);
__STATIC_INLINE void Stopwatch_SetPeriod(uint32_t period);
__STATIC_INLINE uint32_t Stopwatch_GetPeriod(void);
__STATIC_INLINE void Stopwatch_TriggerStart(void);
__STATIC_INLINE void Stopwatch_TriggerReload(void);
__STATIC_INLINE void Stopwatch_TriggerStop(void);
__STATIC_INLINE void Stopwatch_TriggerCapture(void);
__STATIC_INLINE uint32_t Stopwatch_GetInterruptStatus(void);
__STATIC_INLINE void Stopwatch_ClearInterrupt(uint32_t source);
__STATIC_INLINE void Stopwatch_SetInterrupt(uint32_t source);
__STATIC_INLINE void Stopwatch_SetInterruptMask(uint32_t mask);
__STATIC_INLINE uint32_t Stopwatch_GetInterruptMask(void);
__STATIC_INLINE uint32_t Stopwatch_GetInterruptStatusMasked(void);
/** @} general */


/***************************************
*           API Constants
***************************************/

/**
* \addtogroup group_macros
* @{
*/
/** This is a ptr to the base address of the TCPWM instance */
#define Stopwatch_HW                 (Stopwatch_TCPWM__HW)

/** This is a ptr to the base address of the TCPWM CNT instance */
#define Stopwatch_CNT_HW             (Stopwatch_TCPWM__CNT_HW)

/** This is the counter instance number in the selected TCPWM */
#define Stopwatch_CNT_NUM            (Stopwatch_TCPWM__CNT_IDX)

/** This is the bit field representing the counter instance in the selected TCPWM */
#define Stopwatch_CNT_MASK           (1UL << Stopwatch_CNT_NUM)
/** @} group_macros */

#define Stopwatch_INPUT_MODE_MASK    (0x3U)
#define Stopwatch_INPUT_DISABLED     (7U)


/*******************************************************************************
* Function Name: Stopwatch_Init
****************************************************************************//**
*
* Invokes the Cy_TCPWM_Counter_Init() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE cy_en_tcpwm_status_t Stopwatch_Init(cy_stc_tcpwm_counter_config_t const *config)
{
    return(Cy_TCPWM_Counter_Init(Stopwatch_HW, Stopwatch_CNT_NUM, config));
}


/*******************************************************************************
* Function Name: Stopwatch_DeInit
****************************************************************************//**
*
* Invokes the Cy_TCPWM_Counter_DeInit() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void Stopwatch_DeInit(void)                   
{
    Cy_TCPWM_Counter_DeInit(Stopwatch_HW, Stopwatch_CNT_NUM, &Stopwatch_config);
}

/*******************************************************************************
* Function Name: Stopwatch_Enable
****************************************************************************//**
*
* Invokes the Cy_TCPWM_Enable_Multiple() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void Stopwatch_Enable(void)                   
{
    Cy_TCPWM_Enable_Multiple(Stopwatch_HW, Stopwatch_CNT_MASK);
}


/*******************************************************************************
* Function Name: Stopwatch_Disable
****************************************************************************//**
*
* Invokes the Cy_TCPWM_Disable_Multiple() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void Stopwatch_Disable(void)                  
{
    Cy_TCPWM_Disable_Multiple(Stopwatch_HW, Stopwatch_CNT_MASK);
}


/*******************************************************************************
* Function Name: Stopwatch_GetStatus
****************************************************************************//**
*
* Invokes the Cy_TCPWM_Counter_GetStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t Stopwatch_GetStatus(void)                
{
    return(Cy_TCPWM_Counter_GetStatus(Stopwatch_HW, Stopwatch_CNT_NUM));
}


#if(CY_TCPWM_COUNTER_MODE_CAPTURE == Stopwatch_INIT_COMPARE_OR_CAPTURE)
    /*******************************************************************************
    * Function Name: Stopwatch_GetCapture
    ****************************************************************************//**
    *
    * Invokes the Cy_TCPWM_Counter_GetCapture() PDL driver function.
    *
    *******************************************************************************/
    __STATIC_INLINE uint32_t Stopwatch_GetCapture(void)               
    {
        return(Cy_TCPWM_Counter_GetCapture(Stopwatch_HW, Stopwatch_CNT_NUM));
    }


    /*******************************************************************************
    * Function Name: Stopwatch_GetCaptureBuf
    ****************************************************************************//**
    *
    * Invokes the Cy_TCPWM_Counter_GetCaptureBuf() PDL driver function.
    *
    *******************************************************************************/
    __STATIC_INLINE uint32_t Stopwatch_GetCaptureBuf(void)            
    {
        return(Cy_TCPWM_Counter_GetCaptureBuf(Stopwatch_HW, Stopwatch_CNT_NUM));
    }
#else
    /*******************************************************************************
    * Function Name: Stopwatch_SetCompare0
    ****************************************************************************//**
    *
    * Invokes the Cy_TCPWM_Counter_SetCompare0() PDL driver function.
    *
    *******************************************************************************/
    __STATIC_INLINE void Stopwatch_SetCompare0(uint32_t compare0)      
    {
        Cy_TCPWM_Counter_SetCompare0(Stopwatch_HW, Stopwatch_CNT_NUM, compare0);
    }


    /*******************************************************************************
    * Function Name: Stopwatch_GetCompare0
    ****************************************************************************//**
    *
    * Invokes the Cy_TCPWM_Counter_GetCompare0() PDL driver function.
    *
    *******************************************************************************/
    __STATIC_INLINE uint32_t Stopwatch_GetCompare0(void)              
    {
        return(Cy_TCPWM_Counter_GetCompare0(Stopwatch_HW, Stopwatch_CNT_NUM));
    }


    /*******************************************************************************
    * Function Name: Stopwatch_SetCompare1
    ****************************************************************************//**
    *
    * Invokes the Cy_TCPWM_Counter_SetCompare1() PDL driver function.
    *
    *******************************************************************************/
    __STATIC_INLINE void Stopwatch_SetCompare1(uint32_t compare1)      
    {
        Cy_TCPWM_Counter_SetCompare1(Stopwatch_HW, Stopwatch_CNT_NUM, compare1);
    }


    /*******************************************************************************
    * Function Name: Stopwatch_GetCompare1
    ****************************************************************************//**
    *
    * Invokes the Cy_TCPWM_Counter_GetCompare1() PDL driver function.
    *
    *******************************************************************************/
    __STATIC_INLINE uint32_t Stopwatch_GetCompare1(void)              
    {
        return(Cy_TCPWM_Counter_GetCompare1(Stopwatch_HW, Stopwatch_CNT_NUM));
    }


    /*******************************************************************************
    * Function Name: Stopwatch_EnableCompareSwap
    ****************************************************************************//**
    *
    * Invokes the Cy_TCPWM_Counter_EnableCompareSwap() PDL driver function.
    *
    *******************************************************************************/
    __STATIC_INLINE void Stopwatch_EnableCompareSwap(bool enable)  
    {
        Cy_TCPWM_Counter_EnableCompareSwap(Stopwatch_HW, Stopwatch_CNT_NUM, enable);
    }
#endif /* (CY_TCPWM_COUNTER_MODE_CAPTURE == Stopwatch_INIT_COMPARE_OR_CAPTURE) */


/*******************************************************************************
* Function Name: Stopwatch_SetCounter
****************************************************************************//**
*
* Invokes the Cy_TCPWM_Counter_SetCounter() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void Stopwatch_SetCounter(uint32_t count)          
{
    Cy_TCPWM_Counter_SetCounter(Stopwatch_HW, Stopwatch_CNT_NUM, count);
}


/*******************************************************************************
* Function Name: Stopwatch_GetCounter
****************************************************************************//**
*
* Invokes the Cy_TCPWM_Counter_GetCounter() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t Stopwatch_GetCounter(void)               
{
    return(Cy_TCPWM_Counter_GetCounter(Stopwatch_HW, Stopwatch_CNT_NUM));
}


/*******************************************************************************
* Function Name: Stopwatch_SetPeriod
****************************************************************************//**
*
* Invokes the Cy_TCPWM_Counter_SetPeriod() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void Stopwatch_SetPeriod(uint32_t period)          
{
    Cy_TCPWM_Counter_SetPeriod(Stopwatch_HW, Stopwatch_CNT_NUM, period);
}


/*******************************************************************************
* Function Name: Stopwatch_GetPeriod
****************************************************************************//**
*
* Invokes the Cy_TCPWM_Counter_GetPeriod() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t Stopwatch_GetPeriod(void)                
{
    return(Cy_TCPWM_Counter_GetPeriod(Stopwatch_HW, Stopwatch_CNT_NUM));
}


/*******************************************************************************
* Function Name: Stopwatch_TriggerStart
****************************************************************************//**
*
* Invokes the Cy_TCPWM_TriggerStart() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void Stopwatch_TriggerStart(void)             
{
    Cy_TCPWM_TriggerStart(Stopwatch_HW, Stopwatch_CNT_MASK);
}


/*******************************************************************************
* Function Name: Stopwatch_TriggerReload
****************************************************************************//**
*
* Invokes the Cy_TCPWM_TriggerReloadOrIndex() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void Stopwatch_TriggerReload(void)     
{
    Cy_TCPWM_TriggerReloadOrIndex(Stopwatch_HW, Stopwatch_CNT_MASK);
}


/*******************************************************************************
* Function Name: Stopwatch_TriggerStop
****************************************************************************//**
*
* Invokes the Cy_TCPWM_TriggerStopOrKill() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void Stopwatch_TriggerStop(void)
{
    Cy_TCPWM_TriggerStopOrKill(Stopwatch_HW, Stopwatch_CNT_MASK);
}


/*******************************************************************************
* Function Name: Stopwatch_TriggerCapture
****************************************************************************//**
*
* Invokes the Cy_TCPWM_TriggerCaptureOrSwap() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void Stopwatch_TriggerCapture(void)     
{
    Cy_TCPWM_TriggerCaptureOrSwap(Stopwatch_HW, Stopwatch_CNT_MASK);
}


/*******************************************************************************
* Function Name: Stopwatch_GetInterruptStatus
****************************************************************************//**
*
* Invokes the Cy_TCPWM_GetInterruptStatus() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t Stopwatch_GetInterruptStatus(void)       
{
    return(Cy_TCPWM_GetInterruptStatus(Stopwatch_HW, Stopwatch_CNT_NUM));
}


/*******************************************************************************
* Function Name: Stopwatch_ClearInterrupt
****************************************************************************//**
*
* Invokes the Cy_TCPWM_ClearInterrupt() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void Stopwatch_ClearInterrupt(uint32_t source)     
{
    Cy_TCPWM_ClearInterrupt(Stopwatch_HW, Stopwatch_CNT_NUM, source);
}


/*******************************************************************************
* Function Name: Stopwatch_SetInterrupt
****************************************************************************//**
*
* Invokes the Cy_TCPWM_SetInterrupt() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void Stopwatch_SetInterrupt(uint32_t source)
{
    Cy_TCPWM_SetInterrupt(Stopwatch_HW, Stopwatch_CNT_NUM, source);
}


/*******************************************************************************
* Function Name: Stopwatch_SetInterruptMask
****************************************************************************//**
*
* Invokes the Cy_TCPWM_SetInterruptMask() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void Stopwatch_SetInterruptMask(uint32_t mask)     
{
    Cy_TCPWM_SetInterruptMask(Stopwatch_HW, Stopwatch_CNT_NUM, mask);
}


/*******************************************************************************
* Function Name: Stopwatch_GetInterruptMask
****************************************************************************//**
*
* Invokes the Cy_TCPWM_GetInterruptMask() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t Stopwatch_GetInterruptMask(void)         
{
    return(Cy_TCPWM_GetInterruptMask(Stopwatch_HW, Stopwatch_CNT_NUM));
}


/*******************************************************************************
* Function Name: Stopwatch_GetInterruptStatusMasked
****************************************************************************//**
*
* Invokes the Cy_TCPWM_GetInterruptStatusMasked() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t Stopwatch_GetInterruptStatusMasked(void)
{
    return(Cy_TCPWM_GetInterruptStatusMasked(Stopwatch_HW, Stopwatch_CNT_NUM));
}

#endif /* Stopwatch_CY_TCPWM_COUNTER_PDL_H */


/* [] END OF FILE */
