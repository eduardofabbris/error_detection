#ifndef BUFFER_H
#define BUFFER_H

//====================== INCLUDES =====================
#include "gheader.h"

//====================== DEFINES ======================
#define MAX_BUFFER_DATA 1000
#define MAX_BUFFER_DATA_CYCLE 5
#define MAX_REFERENCE_VALUES_CYCLE 5
#define MAX_STRING_SIZE 100

//TOLERANCES
#define WR_TOL 20 //written and read tolerance
#define SR_TOL 10 //slew rate rolerance

//===================== TYPEDEFS =======================
//BUFFER
typedef struct {
    struct {
        uint16_t dtime;
        uint16_t read, written; 
        bool WR_fault;
        bool SR_fault;
    } data[MAX_BUFFER_DATA];
    
    uint16_t dataIndex;
    uint64_t cycleIndex;
    uint16_t SRfaultIndex, WRfaultIndex; 
} BUFFER_DATA;

//REFERENCE VALUES
typedef struct {
  uint16_t maxSlewRate, minSlewRate; 
  uint16_t maxWRDiff, minWRDiff; // written and read difference 
  uint16_t cycleIndex;  
} REFERENCE_VALUES;

//============== FUNCTIONS PROTOTYPES =================
void getReferenceValues(void);
uint16_t verifyFaults(void);

#endif

