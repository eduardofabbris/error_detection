#ifndef BUFFER_H
#define BUFFER_H

//====================== INCLUDES =====================
#include "gheader.h"

//====================== DEFINES ======================
#define MAX_BUFFER_DATA 1000
#define MAX_BUFFER_DATA_CYCLE 5
#define MAX_REFERENCE_VALUES_CYCLE 5
#define MAX_STRING_SIZE 100

#define SR_MAX_DEFAULT_REF 140
#define SR_MIN_DEFAULT_REF 0
    
#define WR_MAX_DEFAULT_REF 23
#define WR_MIN_DEFAULT_REF 0
    
    
    
//TOLERANCES
#define WR_TOL 5 //written and read tolerance, 20
#define SR_TOL 5 //slew rate rolerance, 10

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
uint8_t getReferenceValues(void); 
uint16_t verifyFaults(void);

#endif

