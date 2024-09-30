//====================== INCLUDES =====================
#include "buffer.h"

//================= GLOBAL VARIABLES ==================
extern BUFFER_DATA buffer;
extern REFERENCE_VALUES reference;

//===================== FUNCTIONS =====================

//determines the slew rate and dWR limit values 
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
    
    for(int i=0; i < MAX_BUFFER_DATA; i++){

        //max and min slew rate               
        if(i < MAX_BUFFER_DATA - 1){
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

//**************************************************************************

//compares the values in buffer with the reference values obtained before
uint16_t verifyFaults() 
{
      
    int16   aux = 0,
            SR_faultIndex = 0, 
            WR_faultIndex = 0;
    
    for(int i=0; i < MAX_BUFFER_DATA; i++){

        //verify the slew rate
        if(i < MAX_BUFFER_DATA - 1){
            aux = abs(buffer.data[i+1].read - buffer.data[i].read); 
            if(!(aux >= (int)(reference.minSlewRate - SR_TOL) && aux <= (int)(reference.maxSlewRate + SR_TOL)) ){
                SR_faultIndex++;
                buffer.data[i].SR_fault= true;
            } 
            else {
                buffer.data[i].SR_fault= false; 
            }
            
        }
        
        //verify the difference between what was written and read "dWR"
        aux = abs(buffer.data[i].written - buffer.data[i].read);
        if(!(aux >= (int)(reference.minWRDiff - WR_TOL) && aux <= (int)(reference.maxWRDiff + WR_TOL)) ){
            WR_faultIndex++;
            buffer.data[i].WR_fault = true;
        } 
        else {
            buffer.data[i].WR_fault = false;
        }

    }

#if DEBUG_CODE && (DEBUG_LEVEL == 0)
        char string[MAX_STRING_SIZE] = {0};
        
        sprintf(string, "cycle: %d\r\n", (uint16_t)buffer.cycleIndex);
        UART_PutString(string);
        
        if(SR_faultIndex + WR_faultIndex > 0)
        {
            sprintf(string, "fault count: SR-%i, WR-%i\r\n",SR_faultIndex, WR_faultIndex);
            UART_PutString(string); 
        }
#endif
    
    buffer.SRfaultIndex = SR_faultIndex;
    buffer.WRfaultIndex = WR_faultIndex;

    return SR_faultIndex + WR_faultIndex;   
}
//**************************************************************************




