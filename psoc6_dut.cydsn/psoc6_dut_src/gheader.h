//*shared with all files
#ifndef GHEADER_H
#define GHEADER_H

//====================== INCLUDES =====================
#include "project.h"
#include <stdio.h>
#include <stdlib.h>

//====================== DEFINES ======================
    
// ---> DUT Options <---
    
#define DEBUG_CODE 0
#define DEBUG_LEVEL 1           // Lower the level, more messages are displayed
#define FORCE_KIT_RESET 0       // Kit will reset after a buffer read, independent if an error was found
     
#define FAULT_EMULATION_EN 1

#define EVALUATE_ALL_BUFFERS_EN 1
#define DYNAMIC_REF_VALUES_EN 0
#define EVALUATE_REF_VALUES_EN 0
    

//============== FUNCTIONS PROTOTYPES =================
void ADC_ISR_Callback(void);
void initDevices();


#endif
