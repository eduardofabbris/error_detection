/*******************************************************************************
* @filename : gheader.h
* @brief    : Global header shared with all files
*
* MIT License
*
* Copyright (c) 2024 eduardofabbris
* See the LICENSE file for details.
********************************************************************************/
#ifndef GHEADER_H
#define GHEADER_H

/*********************************************************
* Includes
*********************************************************/
#include "project.h"
#include <stdio.h>
#include <stdlib.h>

/*********************************************************
* Defines
*********************************************************/

// ---> DUT Options <---
#define DEBUG_CODE 0            // Enable debug
#define DEBUG_LEVEL 1           // Lower the level, more messages are displayed
#define FORCE_KIT_RESET 0       // Kit will reset after a buffer read, independent if an error was found

#define FAULT_EMULATION_EN 0      // Enable fault emulation or debug menu features in monitoring software
#define EVALUATE_ALL_BUFFERS_EN 1 // Evaluate only a number of buffers defnined by MAX_BUFFER_DATA_CYCLE if disabled
#define DYNAMIC_REF_VALUES_EN 0   // Use dynamic reference values (always evaluated at the start)
#define EVALUATE_REF_VALUES_EN 0  // Enable reference value evaluation

/*********************************************************
* Function Prototypes
*********************************************************/

void ADC_ISR_Callback(void);
void initDevices();

#endif
