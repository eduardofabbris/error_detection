/*******************************************************************************
* @filename : buffer.c
* @brief    : Buffer handling
*
* MIT License
*
* Copyright (c) 2024 eduardofabbris
* See the LICENSE file for details.
********************************************************************************/

/*********************************************************
* Includes
*********************************************************/
#include "buffer.h"

/*********************************************************
* Global Variables
*********************************************************/
extern BUFFER_DATA buffer;
extern BUFFER_DATA bufferA, bufferB;   // Backup buffers
extern REFERENCE_VALUES reference;

/*********************************************************
* Function Definitions
*********************************************************/

/**
* @brief Determines the slew rate and dWR limit values
* @retval : True if the values were updated
*/
uint8_t getReferenceValues(void)
{
    int16 aux = 0;
    uint8_t update_flag = 0;

    // First time initial values
    if(reference.cycleIndex == 0)
    {
        update_flag = 1;

        reference.maxSlewRate
        = reference.minSlewRate
        = abs( buffer.data[1].read - buffer.data[0].read);

        reference.maxWRDiff
        = reference.minWRDiff
        = abs(buffer.data[0].written - buffer.data[0].read);
    }

    for(int i=0; i < MAX_BUFFER_DATA; i++)
    {

        // Max and min slew rate
        if(i < MAX_BUFFER_DATA - 1)
        {
            aux = abs(buffer.data[i+1].read - buffer.data[i].read);

            if(aux > reference.maxSlewRate)
            {
                update_flag = 1;
                reference.maxSlewRate = aux;
            }
            if(aux < reference.minSlewRate)
            {
                update_flag = 1;
                reference.minSlewRate = aux;
            }
        }

        // Max and min written and read diff
        aux = abs(buffer.data[i].written - buffer.data[i].read);

        if(aux > reference.maxWRDiff)
        {
            update_flag = 1;
            reference.maxWRDiff = aux;
        }
        if(aux < reference.minWRDiff)
        {
            update_flag = 1;
            reference.minWRDiff = aux;
        }

    }

    return update_flag;
}
//**************************************************************************************

/**
* @brief  Simple verification. Verifys if all three values are equal
* @retval : True if all values are the same
*/
static uint8_t is_equal(uint16_t val_a, uint16_t val_b, uint16_t val_c)
{
    uint8_t result = 1;

    if (val_a == val_b)
    {
        if (val_b != val_c)
        {
            result = 0;
        }
    }
    else
    {
        result = 0;
    }

    return result;
}
//**************************************************************************************

/**
* @brief  Compares buffer values with reference values
* @retval : True if a error was found
*/
uint16_t verifyFaults()
{
    uint8_t found_error = 0;
    int16   aux = 0,
            SR_faultIndex = 0,
            WR_faultIndex = 0;

    for(int i=0; i < MAX_BUFFER_DATA; i++)
    {
        // Reset values
        buffer.data[i].fault_descriptor = 0;

        // Verify backup data
        if ( !is_equal(buffer.data[i].read, bufferA.data[i].read,  bufferB.data[i].read) )
        {
            found_error = 1;
            buffer.data[i].fault_descriptor += (1 << 7);
        }
        if ( !is_equal(buffer.data[i].written, bufferA.data[i].written,  bufferB.data[i].written) )
        {
            found_error = 1;
            buffer.data[i].fault_descriptor += (1 << 6);
        }
        if ( !is_equal(buffer.data[i].dtime, bufferA.data[i].dtime,  bufferB.data[i].dtime) )
        {
            found_error = 1;
            buffer.data[i].fault_descriptor += (1 << 5);
        }

        // Verify dtime difference
        if (abs(buffer.data[i].dtime - DTIME_DEFAULT_REF) >= DT_TOL)
        {
            found_error = 1;
            buffer.data[i].fault_descriptor += (1 << 4);
        }


        // Verify slew rate
        if(i < MAX_BUFFER_DATA - 1){
            aux = abs(buffer.data[i+1].read - buffer.data[i].read);
            if(!(aux >= (int)(reference.minSlewRate - SR_TOL) && aux <= (int)(reference.maxSlewRate + SR_TOL)) ){
                found_error = 1;
                SR_faultIndex++;
                buffer.data[i].fault_descriptor += (1 << 1);
            }
        }

        // Verify the difference between what was written and read "dWR"
        aux = abs(buffer.data[i].written - buffer.data[i].read);
        if(!(aux >= (int)(reference.minWRDiff - WR_TOL) && aux <= (int)(reference.maxWRDiff + WR_TOL)) ){
            found_error = 1;
            WR_faultIndex++;
            buffer.data[i].fault_descriptor += (1 << 0);
        }
    }

#if DEBUG_CODE
    char string[MAX_STRING_SIZE] = {0};

    if (DEBUG_LEVEL == 0)
    {
        sprintf(string, "cycle: %d\r\n", (uint16_t)buffer.cycleIndex);
        UART_PutString(string);
        while(!UART_IsTxComplete());
    }

    if (DEBUG_LEVEL <= 1)
    {
        if(SR_faultIndex + WR_faultIndex > 0)
        {
            sprintf(string, "fault count: SR-%i, WR-%i\r\n",SR_faultIndex, WR_faultIndex);
            UART_PutString(string);
            while(!UART_IsTxComplete());
        }

    }

#endif

    buffer.SRfaultIndex = SR_faultIndex;
    buffer.WRfaultIndex = WR_faultIndex;

    return found_error;
}
//**************************************************************************************

