//+============================================================
//| Projeto iniciacao cientifica PSoc6 2023 - Eduardo Fabbris
//|
//+============================================================

//====================== INCLUDES =====================

#include "psoc6_dut_src\gheader.h"
#include "psoc6_dut_src\buffer.h"
#include "psoc6_dut_src\trainingData.h"
#include "psoc6_dut_src\serial.h"

//================= GLOBAL VARIABLES ==================
BUFFER_DATA buffer;
REFERENCE_VALUES reference;
extern uint32_t triangularWaveLUT;


//===================== FUNCTIONS =====================

static void UART_print_ref(void)
{
    char string[MAX_STRING_SIZE] = {0};
    
    sprintf
    (
        string, "ref: maxWR=%d, minWR=%d, maxslew=%d, minslew=%d\r\n",
        reference.maxWRDiff,  
        reference.minWRDiff, 
        reference.maxSlewRate, 
        reference.minSlewRate
    );

    UART_PutString(string);  
    while(!UART_IsTxComplete());
}


/**
* @brief  Entry code
* @retval : none
*/
int main(void) {
    
    initDevices();
/*#if PSOC_MONITOR_ON
    //start warning led
    Cy_GPIO_Write(RED_LED_0_PORT, RED_LED_0_NUM, 0); //pull-up
    
    uint8_t buff = 0, n=0;
    do{
        while(UART_Get() != 'H'){ //waits host message            
            UART_PutString("MW"); //warns host that it's wating
            while(!UART_IsTxComplete());
            CyDelay(500);           
        }
        
        n = buff = 0;
        while( (buff = UART_Get() )!= 'S' && n < 10){
            CyDelayUs(10);
            n++;
        }
       
    }while(buff != 'S'); //waits start signal
    
    CyDelay(1);
    UART_PutString("MS"); //confirm start to host
    while(!UART_IsTxComplete());
    Cy_GPIO_Write(RED_LED_0_PORT, RED_LED_0_NUM, 1);//pull-up
#endif
*/
    Cy_GPIO_Write(GREEN_LED_0_PORT, GREEN_LED_0_NUM, 0); // Turn on green led (pull-up)
    buffer.cycleIndex = 0;
    
    if (DYNAMIC_REF_VALUES_EN == 0)
    {
        reference.maxSlewRate = SR_MAX_DEFAULT_REF;
        reference.minSlewRate = SR_MIN_DEFAULT_REF;
        
        reference.maxWRDiff = WR_MAX_DEFAULT_REF;
        reference.minWRDiff = WR_MIN_DEFAULT_REF;
    }
     
    // Start bias
    Timestamp_Timer_TriggerStart(); // Start timestamp timer
    Stopwatch_TriggerStart(); // starts timer
    ADC_StartConvert();       // starts ADC conversion         
    

    while(1);
}

/**
* @brief  ADC interrupt. This function is called every time a new conversion is finished
* @retval : none
*/
void ADC_ISR_Callback(void)
{
    static uint32_t UART_aliveSignalCounter = 0;
    static uint16_t nextValue;
    static bool firstConv = true;


    // Stop stopwatch
    Stopwatch_TriggerStop();
      
    // If buffer is not full yet
    if(buffer.dataIndex < MAX_BUFFER_DATA)
    {
        // First value is neglected 
        if(!firstConv) // TODO: verify implicantions    
        {
            // Get DAC value
            buffer.data[buffer.dataIndex].written = nextValue;           
            // Get ADC value
            buffer.data[buffer.dataIndex].read =  ADC_GetResult16(0);
            // Gets time            
            buffer.data[buffer.dataIndex].dtime = Stopwatch_GetCounter();
            
            buffer.dataIndex++;
        }  
        else
        {
            firstConv = false;    
        }
                
    }  
    else {
        // Alive signal
        Cy_GPIO_Inv(ALIVE_SIGNAL_0_PORT, ALIVE_SIGNAL_0_NUM);
        
#if !EVALUATE_REF_VALUES_EN              
        // Serial alive message
        if(UART_aliveSignalCounter < 500)
        {
            UART_aliveSignalCounter++;
        }
        else
        {
            // UART DUT Alive
            UART_PutString("DA");   
            // Visual Alive
            Cy_GPIO_Inv(GREEN_LED_0_PORT, GREEN_LED_0_NUM);            
            UART_aliveSignalCounter = 0;   
        }
#endif      
        // Reference values
        if( (reference.cycleIndex < MAX_REFERENCE_VALUES_CYCLE) && DYNAMIC_REF_VALUES_EN ) 
        {          
            uint8_t updated_flag = getReferenceValues();
#if DEBUG_CODE    
                // Print buffer
                UARTPrintData(); 
#endif
            // Always check for new reference values
            if (EVALUATE_REF_VALUES_EN)
            {
                if (updated_flag)
                {
                    UART_print_ref();
                }
                reference.cycleIndex = 1;

            }
            else
            {
                reference.cycleIndex++;
            }
        
        }
        // If debug mode, only send first 5 buffers via UART
        else if(buffer.cycleIndex < MAX_BUFFER_DATA_CYCLE || !DEBUG_CODE) 
        {
            // If a error is found send all data
            if(verifyFaults() || DEBUG_CODE)
            {
                // Stop timestamp timer
                Timestamp_Timer_TriggerStop();
#if DEBUG_CODE   
                UARTPrintData();                
                // Reset visual warning    
                for(int i=0; i <2*3; i++)
                {
                    Cy_GPIO_Inv(RED_LED_0_PORT, RED_LED_0_NUM);
                    CyDelay(250);   
                }
#else

                UART_print_ref();
                UART_send_buffer();
#endif
                // Reset kit with watchdog
                WD_reset_Start();
            }            
            buffer.cycleIndex++;
        }       

        buffer.dataIndex = 0; 
        firstConv = true;
        //Cy_GPIO_Write(ALIVE_SIGNAL_0_PORT, ALIVE_SIGNAL_0_NUM, 0);

    }

    //while(CTDAC0->CTDAC_VAL_NXT == nextValue);
    
    // Reload stopwatch
    Stopwatch_TriggerReload();
       
    // Start stopwatch
    Stopwatch_TriggerStart();    
    
    // Store DAC's next value
    nextValue = CTDAC0->CTDAC_VAL_NXT;
    
#if FAULT_EMULATION_EN
    if(!Cy_GPIO_Read(USER_BUTTON_PORT, USER_BUTTON_0_NUM))
    {
        CTDAC0->CTDAC_VAL_NXT ^= (1 << 7);
    }
#endif 

    while(nextValue != CTDAC0->CTDAC_VAL);
    // Start another conversion
    ADC_StartConvert();

}
//**************************************************************************

/**
* @brief  Initialize peripherals
* @retval : none
*/
void initDevices(){

    // Enable global interrupts.
    __enable_irq();
    
    //UART
    UART_Start();

    // Timer
    Stopwatch_Init(&Stopwatch_config);
    Stopwatch_Enable();
    
    // Timestamp Timer
    Timestamp_Timer_Init(&Timestamp_Timer_config);
    Timestamp_Timer_Enable();

    //DMA
    DMA1_Start(&triangularWaveLUT, (uint32_t *)&(CTDAC0->CTDAC_VAL_NXT));

    //DAC
    DAC_Start();  
    
    //ADC
    ADC_Start();   
    ADC_IRQ_Enable();    
}
//**************************************************************************







