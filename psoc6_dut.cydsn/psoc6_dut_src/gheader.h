//*shared with all files
#ifndef GHEADER_H
#define GHEADER_H

//====================== INCLUDES =====================
#include "project.h"
#include <stdio.h>
#include <stdlib.h>

//====================== DEFINES ======================
#define DEBUG_CODE 0
#define LUT_DATA_SIZE 500
#define PSOC_MONITOR_ON 1
#define DYNAMIC_REF_VALUES_EN 0


//============== FUNCTIONS PROTOTYPES =================
void ADC_ISR_Callback(void);
void initDevices();


#endif
