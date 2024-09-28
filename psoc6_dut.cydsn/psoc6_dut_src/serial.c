#include "serial.h"

//====================== DEFINES ======================
#define MAX_STRING_SIZE 100

//================= GLOBAL VARIABLES ==================
extern BUFFER_DATA buffer;
extern REFERENCE_VALUES reference;

//===================== FUNCTIONS =====================


void UARTPrintData(void) {
    
#if PSOC_MONITOR_ON

    for(int i=0; i < MAX_BUFFER_DATA; i++){  
        Cy_GPIO_Write(ALIVE_SIGNAL_0_PORT, ALIVE_SIGNAL_0_NUM, 1);
        uint8_t aux = 0xFF & ~(buffer.data[i].WR_fault + (buffer.data[i].SR_fault << 1) );       

        uint16_t writeBuffer[] = {
            buffer.data[i].read,
            buffer.data[i].dtime,
            buffer.data[i].written,
            aux //1byte only
        };
        
        UART_PutString("MD");
        while(!UART_IsTxComplete());
        
        UART_PutArrayBlocking(writeBuffer, sizeof(writeBuffer)); 
        while(!UART_IsTxComplete());
        
        Cy_GPIO_Write(ALIVE_SIGNAL_0_PORT, ALIVE_SIGNAL_0_NUM, 0);
    }
    
    CyDelay(100);
    
    
#else
    char string[MAX_STRING_SIZE] = {0}; 
    static bool printHeader = true;
    

    //reference values
    if(printHeader || DEBUG_CODE){
        sprintf(string, "Param: maxWR=%i, minWR=%i, maxslew=%i, minslew=%i\r\n",
                     reference.maxWRDiff,  
                     reference.minWRDiff, 
                     reference.maxSlewRate, 
                     reference.minSlewRate);

        UART_PutString(string);  
        while(!UART_IsTxComplete());
        
        UART_PutString("ADC \t DAC \t deltaWR \t dt \t WRfault \t SRfault\r\n");
        while(!UART_IsTxComplete());
        
        printHeader = false;
    }

    //buffer 
    //sprintf(string, "Ciclo: %u\r\n", buffer.cycleIndex);
    //UART_PutString(string);
    //while(!UART_IsTxComplete());
    
    
    sprintf(string, "Fault count: SR-%i \t WR-%i  \r\n",buffer.SRfaultIndex, buffer.WRfaultIndex);
    UART_PutString(string);
    while(!UART_IsTxComplete());

    
    
    for(int i=0; i < MAX_BUFFER_DATA; i++){  
             
        sprintf(string, "%i \t %i \t %i \t %u \t %d \t %d\r\n", 
                     buffer.data[i].read, 
                     buffer.data[i].written, 
                     abs( buffer.data[i].read-buffer.data[i].written), 
                     (uint16_t) buffer.data[i].dtime, 
                     buffer.data[i].WR_fault, 
                     buffer.data[i].SR_fault);

        UART_PutString(string); 
        while(!UART_IsTxComplete());
    }
    UART_PutString("\r\n");
    while(!UART_IsTxComplete());
    
#endif  
}

//****************************************************************************
