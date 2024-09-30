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


/**
* @brief  Entry code
* @retval : none
*/
int main(void) 
{
    
    initDevices();

    // Turn on green led (pull-up)
    Cy_GPIO_Write(GREEN_LED_0_PORT, GREEN_LED_0_NUM, 0);
    
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
#if !DEBUG_CODE
            // UART DUT Alive
            UART_PutString("DA");   
#endif
            // Visual Alive
            Cy_GPIO_Inv(GREEN_LED_0_PORT, GREEN_LED_0_NUM);            
            UART_aliveSignalCounter = 0;   
        }
#endif      
        // Reference values
        if( (reference.cycleIndex < MAX_REFERENCE_VALUES_CYCLE) && DYNAMIC_REF_VALUES_EN ) 
        {          
            uint8_t updated_flag = getReferenceValues();

            // Always check for new reference values
            if (EVALUATE_REF_VALUES_EN)
            {
                if (updated_flag)
                {
                    UART_print_data(0, 1, 1);  
                }
                reference.cycleIndex = 1;

            }
            else
            {
                reference.cycleIndex++;
            }
        
        }
        // Verify buffer
        else if(buffer.cycleIndex < MAX_BUFFER_DATA_CYCLE || EVALUATE_ALL_BUFFERS_EN) 
        {
            // Stop timestamp timer
            Timestamp_Timer_TriggerStop();
                
            // If a error is found send all data
            if(verifyFaults() || FORCE_KIT_RESET)
            {
#if DEBUG_CODE   
                UART_print_data(0, 0, 1);                 
                // Reset visual warning    
                for(int i=0; i <2*3; i++)
                {
                    Cy_GPIO_Inv(RED_LED_0_PORT, RED_LED_0_NUM);
                    CyDelay(250);   
                }
#else
                UART_send_buffer();
#endif
                // Reset kit with watchdog
                WD_reset_Start();
            }            
            buffer.cycleIndex++;
            // Restarts timestamp timer
            Timestamp_Timer_TriggerStart();
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
    static bool fault_latch = false;
    static uint16_t fixed_sample = 0;
    
    if(!Cy_GPIO_Read(USER_BUTTON_PORT, USER_BUTTON_0_NUM) && !fault_latch)
    {
        fixed_sample = CTDAC0->CTDAC_VAL_NXT;
        CTDAC0->CTDAC_VAL_NXT ^= (1 << 11);
        fault_latch = true;       
    }
    
    if (fault_latch && FIXED_SAMPLE_ERROR_EN)
    {
        CTDAC0->CTDAC_VAL_NXT = fixed_sample;
        CTDAC0->CTDAC_VAL     = fixed_sample;
    }
#endif 

    while((nextValue != CTDAC0->CTDAC_VAL) && !FAULT_EMULATION_EN);
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







