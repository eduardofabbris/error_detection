/***************************************************************************//**
* \file DMA.c
* \version 2.0
*
*  This file provides the source code to the API for the
*  DMA component.
*
********************************************************************************
* \copyright
* Copyright 2016-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "DMA.h"

#if defined(__cplusplus)
extern "C" {
#endif

/* Generated code */
const cy_stc_dma_descriptor_config_t DMA_Descriptor_1_config =
{
    .retrigger       = CY_DMA_RETRIG_IM,
    .interruptType   = CY_DMA_1ELEMENT,
    .triggerOutType  = CY_DMA_1ELEMENT,
    .channelState    = CY_DMA_CHANNEL_ENABLED,
    .triggerInType   = CY_DMA_1ELEMENT,
    .dataSize        = CY_DMA_WORD,
    .srcTransferSize = CY_DMA_TRANSFER_SIZE_DATA,
    .dstTransferSize = CY_DMA_TRANSFER_SIZE_DATA,
    .descriptorType  = CY_DMA_2D_TRANSFER,
    .srcAddress      = NULL,
    .dstAddress      = NULL,
    .srcXincrement   = 1L,
    .dstXincrement   = 0L,
    .xCount          = 100UL,
    .srcYincrement   = 100L,
    .dstYincrement   = 0L,
    .yCount          = 5UL,
    .nextDescriptor  = &DMA_Descriptor_2
};

cy_stc_dma_descriptor_t DMA_Descriptor_1 =
{
    .ctl = 0UL,
    .src = 0UL,
    .dst = 0UL,
    .xCtl = 0UL,
    .yCtl = 0UL,
    .nextPtr = 0UL
};

const cy_stc_dma_descriptor_config_t DMA_Descriptor_2_config =
{
    .retrigger       = CY_DMA_RETRIG_IM,
    .interruptType   = CY_DMA_1ELEMENT,
    .triggerOutType  = CY_DMA_1ELEMENT,
    .channelState    = CY_DMA_CHANNEL_ENABLED,
    .triggerInType   = CY_DMA_1ELEMENT,
    .dataSize        = CY_DMA_WORD,
    .srcTransferSize = CY_DMA_TRANSFER_SIZE_DATA,
    .dstTransferSize = CY_DMA_TRANSFER_SIZE_DATA,
    .descriptorType  = CY_DMA_2D_TRANSFER,
    .srcAddress      = NULL,
    .dstAddress      = NULL,
    .srcXincrement   = 1L,
    .dstXincrement   = 0L,
    .xCount          = 100UL,
    .srcYincrement   = 100L,
    .dstYincrement   = 0L,
    .yCount          = 5UL,
    .nextDescriptor  = &DMA_Descriptor_1
};

cy_stc_dma_descriptor_t DMA_Descriptor_2 =
{
    .ctl = 0UL,
    .src = 0UL,
    .dst = 0UL,
    .xCtl = 0UL,
    .yCtl = 0UL,
    .nextPtr = 0UL
};


/** DMA_initVar indicates whether the DMA 
*  component has been initialized. The variable is initialized to 0 
*  and set to 1 the first time DMA_Start() is called. This allows 
*  the component to restart without reinitialization after the first 
*  call to the DMA_Start() routine.
*
*  If re-initialization of the component is required, then the 
*  DMA_Init() function can be called before the 
*  DMA_Start() or DMA_ChEnable() function.
*/
uint8 DMA_initVar = 0u;


/*******************************************************************************
* Function Name: DMA_Start
****************************************************************************//**
*
* Based on the settings for descriptor in the customizer this function runs the
* DMA_Descriptor_Init() and then initializes the channel using
* DMA_Chnl_Init(). Enables the DMA block using the DMA_Chnl_Enable().
*  
*******************************************************************************/
void DMA_Start(void const * srcAddress, void const * dstAddress)
{
    if (0U == DMA_initVar)
    {
        DMA_Init();
        DMA_initVar = 1u;
    }
    
    Cy_DMA_Descriptor_SetSrcAddress(&DMA_Descriptor_1, srcAddress);
    Cy_DMA_Descriptor_SetDstAddress(&DMA_Descriptor_1, dstAddress);
    Cy_DMA_Channel_Enable(DMA_HW, DMA_DW_CHANNEL);
}


/*******************************************************************************
* Function Name: DMA_Init
****************************************************************************//**
*
* Based on the settings for the descriptor in the customizer this function runs the
* DMA_Descriptor_Init() and then initializes the channel using
* DMA_Chnl_Init().
*  
*******************************************************************************/
void DMA_Init(void)
{
    cy_stc_dma_channel_config_t channelConfig;

    /* Init all descriptors */
    (void)Cy_DMA_Descriptor_Init(&DMA_Descriptor_1, &DMA_Descriptor_1_config);
    (void)Cy_DMA_Descriptor_Init(&DMA_Descriptor_2, &DMA_Descriptor_2_config);


    channelConfig.descriptor  = &DMA_Descriptor_1;
    channelConfig.preemptable = DMA_PREEMPTABLE;
    channelConfig.priority    = DMA_PRIORITY;
    channelConfig.enable      = false;
    channelConfig.bufferable  = DMA_BUFFERABLE;

    (void)Cy_DMA_Channel_Init(DMA_HW, DMA_DW_CHANNEL, &channelConfig);

    Cy_DMA_Enable(DMA_HW);
}


#if defined(__cplusplus)
}
#endif

/* [] END OF FILE */
