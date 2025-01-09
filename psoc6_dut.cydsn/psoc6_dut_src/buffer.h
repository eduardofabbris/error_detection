/*******************************************************************************
* @filename : buffer.h
* @brief    : Buffer handling header
*
* MIT License
*
* Copyright (c) 2024 eduardofabbris
* See the LICENSE file for details.
********************************************************************************/
#ifndef BUFFER_H
#define BUFFER_H

/*********************************************************
* Includes
*********************************************************/
#include "gheader.h"

/*********************************************************
* Defines
*********************************************************/
#define MAX_BUFFER_DATA 1000
#define MAX_BUFFER_DATA_CYCLE 5
#define MAX_REFERENCE_VALUES_CYCLE 5
#define MAX_STRING_SIZE 100

// Static Reference Values
#define SR_MAX_DEFAULT_REF 140
#define SR_MIN_DEFAULT_REF 0

//#define WR_MAX_DEFAULT_REF 35
#define WR_MAX_DEFAULT_REF 23
#define WR_MIN_DEFAULT_REF 0

#define DTIME_DEFAULT_REF 12

//TOLERANCES
#define WR_TOL 5    // written and read tolerance
#define SR_TOL 5    // slew rate trolerance
#define DT_TOL 10   // time difference tolerance

/*********************************************************
* Typedefs
*********************************************************/
//BUFFER
typedef struct {
    struct {
        uint16_t dtime;
        uint16_t read, written;
        uint8_t  fault_descriptor;
    } data[MAX_BUFFER_DATA];

    uint16_t dataIndex;
    uint64_t cycleIndex;
    uint16_t SRfaultIndex, WRfaultIndex;
} BUFFER_DATA;

//REFERENCE VALUES
typedef struct {
  uint16_t maxSlewRate, minSlewRate;
  uint16_t maxWRDiff, minWRDiff;
  uint16_t cycleIndex;
} REFERENCE_VALUES;

/*********************************************************
* Function Prototypes
*********************************************************/

uint8_t getReferenceValues(void);
uint16_t verifyFaults(void);

#endif

