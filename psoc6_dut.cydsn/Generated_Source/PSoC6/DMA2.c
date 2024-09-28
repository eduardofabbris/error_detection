/***************************************************************************//**
* \file DMA2.c
* \version 2.0
*
*  This file provides the source code to the API for the
*  DMA2 component.
*
********************************************************************************
* \copyright
* Copyright 2016-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "DMA2.h"

#if defined(__cplusplus)
extern "C" {
#endif

/* Generated code */
const cy_stc_dma_descriptor_config_t DMA2_Descriptor_1_config =
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
    .nextDescriptor  = &DMA2_Descriptor_1
};

cy_stc_dma_descriptor_t DMA2_Descriptor_1 =
{
    .ctl = 0UL,
    .src = 0UL,
    .dst = 0UL,
    .xCtl = 0UL,
    .yCtl = 0UL,
    .nextPtr = 0UL
};


/** DMA2_initVar indicates whether the DMA2 
*  component has been initialized. The variable is initialized to 0 
*  and set to 1 the first time DMA2_Start() is called. This allows 
*  the component to restart without reinitialization after the first 
*  call to the DMA2_Start() routine.
*
*  If re-initialization of the component is required, then the 
*  DMA2_Init() function can be called before the 
*  DMA2_Start() or DMA2_ChEnable() function.
*/
uint8 DMA2_initVar = 0u;


/*******************************************************************************
* Function Name: DMA2_Start
****************************************************************************//**
*
* Based on the settings for descriptor in the customizer this function runs the
* DMA_Descriptor_Init() and then initializes the channel using
* DMA_Chnl_Init(). Enables the DMA2 block using the DMA_Chnl_Enable().
*  
*******************************************************************************/
void DMA2_Start(void const * srcAddress, void const * dstAddress)
{
    if (0U == DMA2_initVar)
    {
        DMA2_Init();
        DMA2_initVar = 1u;
    }
    
    Cy_DMA_Descriptor_SetSrcAddress(&DMA2_Descriptor_1, srcAddress);
    Cy_DMA_Descriptor_SetDstAddress(&DMA2_Descriptor_1, dstAddress);
    Cy_DMA_Channel_Enable(DMA2_HW, DMA2_DW_CHANNEL);
}


/*******************************************************************************
* Function Name: DMA2_Init
****************************************************************************//**
*
* Based on the settings for the descriptor in the customizer this function runs the
* DMA_Descriptor_Init() and then initializes the channel using
* DMA_Chnl_Init().
*  
*******************************************************************************/
void DMA2_Init(void)
{
    cy_stc_dma_channel_config_t channelConfig;

    /* Init all descriptors */
    (void)Cy_DMA_Descriptor_Init(&DMA2_Descriptor_1, &DMA2_Descriptor_1_config);


    channelConfig.descriptor  = &DMA2_Descriptor_1;
    channelConfig.preemptable = DMA2_PREEMPTABLE;
    channelConfig.priority    = DMA2_PRIORITY;
    channelConfig.enable      = false;
    channelConfig.bufferable  = DMA2_BUFFERABLE;

    (void)Cy_DMA_Channel_Init(DMA2_HW, DMA2_DW_CHANNEL, &channelConfig);

    Cy_DMA_Enable(DMA2_HW);
}


#if defined(__cplusplus)
}
#endif

/* [] END OF FILE */
