/*******************************************************************************
* @filename : main_cm4.c
* @brief    : Error detection system interrupt routine
*
* MIT License
*
* Copyright (c) 2024 eduardofabbris
* See the LICENSE file for details.
********************************************************************************/

/*********************************************************
* Includes
*********************************************************/
#include "psoc6_dut_src\gheader.h"
#include "psoc6_dut_src\buffer.h"
#include "psoc6_dut_src\trainingData.h"
#include "psoc6_dut_src\serial.h"

/*********************************************************
* Global Variables
*********************************************************/
BUFFER_DATA buffer;             // Main buffer
BUFFER_DATA bufferA, bufferB;   // Backup buffers
REFERENCE_VALUES reference;
extern uint32_t triangularWaveLUT;

/*********************************************************
* Function Definitions
*********************************************************/

#if FAULT_EMULATION_EN
/**
* @brief  Error injection for Debug
* @retval : none
*/
static uint16_t fixed_sample       = 0;
static uint8_t trigger_fixed_smpl  = 0,
               trigger_single_smpl = 0,
               trigger_hang        = 0,
               trigger_time        = 0,
               trigger_buf_mem     = 0,
               trigger_stop_uart   = 0;

static void error_injector_rx(void)
{
    enum error_injector_fsm_st
    {
      FSM_IDLE_ST,
      FSM_ERROR_SEL_ST
    };
    static int fsm_st = FSM_IDLE_ST;

    uint8_t rx_byte = 0;
    if ( UART_GetNumInRxFifo() > 0 )
    {
        rx_byte = UART_Get();
        switch(fsm_st)
        {
            // Idle state
            case FSM_IDLE_ST:
                // Error Inject ID
                if (rx_byte == 'E')
                {
                    fsm_st = FSM_ERROR_SEL_ST;
                }
            break;

            // Error select state
            case FSM_ERROR_SEL_ST:
                // Time difference between samples
                if (rx_byte == 't')
                {
                    trigger_time = 1;
                }
                // Single sample or upset
                else if (rx_byte == 'u')
                {
                    trigger_single_smpl = 1;
                }
                // Fixed sample or latch-up
                else if (rx_byte == 'l')
                {
                    fixed_sample = CTDAC0->CTDAC_VAL_NXT;
                    trigger_fixed_smpl = 1;
                }
                // Core hang
                else if (rx_byte == 'h')
                {
                    trigger_hang = 1;
                }
                // Memory buffer
                else if (rx_byte == 'm')
                {
                    trigger_buf_mem = 1;
                }
                // Stop serial port
                else if (rx_byte == 's')
                {
                    trigger_stop_uart = 1;
                }
                fsm_st = FSM_IDLE_ST;
            break;

                // Invalid state
            default:
                fsm_st = FSM_IDLE_ST;
            break;
        }
    }
}
#endif
//**************************************************************************************

/**
* @brief  Entry code
* @retval : none
*/
int main(void)
{

    initDevices();

    // Initial UART DUT Alive
    UART_PutString("DA");

    // Turn on green led (pull-up)
    Cy_GPIO_Write(GREEN_LED_0_PORT, GREEN_LED_0_NUM, 0);

    buffer.cycleIndex  =
    bufferA.cycleIndex =
    bufferB.cycleIndex = 0;

    if (DYNAMIC_REF_VALUES_EN == 0)
    {
/**
* It was observed that the logic added to enable
* this feature slightly changes the reference value of SR_MAX.
* So, in order to be able to emulate fault injections without
* unwanted buffers, we need to increase a little this value.
*/
#if FAULT_EMULATION_EN
        reference.maxSlewRate = SR_MAX_DEFAULT_REF + 9;
#else
        reference.maxSlewRate = SR_MAX_DEFAULT_REF;
#endif
        reference.minSlewRate = SR_MIN_DEFAULT_REF;

        reference.maxWRDiff = WR_MAX_DEFAULT_REF;
        reference.minWRDiff = WR_MIN_DEFAULT_REF;
    }

    // Start bias
    Timestamp_Timer_TriggerStart(); // Start timestamp timer
    Stopwatch_TriggerStart();       // Starts stopwatchtimer
    ADC_StartConvert();             // Start ADC conversion

    while(1)
    {
#if FAULT_EMULATION_EN
        error_injector_rx();
        CyDelay(10);
# endif
    }
}
//**************************************************************************************

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
        // First value is neglected because we don't have nextValue variable yet
        if(!firstConv)
        {
            // Get DAC value
            buffer.data[buffer.dataIndex].written  =
            bufferA.data[buffer.dataIndex].written =
            bufferB.data[buffer.dataIndex].written = nextValue;
            // Get ADC value
            buffer.data[buffer.dataIndex].read  =
            bufferA.data[buffer.dataIndex].read =
            bufferB.data[buffer.dataIndex].read =  ADC_GetResult16(0);
            // Gets time
            buffer.data[buffer.dataIndex].dtime  =
            bufferA.data[buffer.dataIndex].dtime =
            bufferB.data[buffer.dataIndex].dtime = Stopwatch_GetCounter();

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

#if FAULT_EMULATION_EN
            if (!trigger_stop_uart)
            {
                // UART DUT Alive
                UART_PutString("DA");
            }
#else
            // UART DUT Alive
            UART_PutString("DA");
#endif // FAULT_EMULATION_EN

#endif // !DEBUG_CODE
            // Visual Alive
            Cy_GPIO_Inv(GREEN_LED_0_PORT, GREEN_LED_0_NUM);
            UART_aliveSignalCounter = 0;
        }
#endif // !EVALUATE_REF_VALUES_EN

        // Reference values
        if( DYNAMIC_REF_VALUES_EN && (reference.cycleIndex < MAX_REFERENCE_VALUES_CYCLE))
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
        else if(EVALUATE_ALL_BUFFERS_EN || (buffer.cycleIndex < MAX_BUFFER_DATA_CYCLE))
        {
            // Stop timestamp timer
            Timestamp_Timer_TriggerStop();

#if FAULT_EMULATION_EN
            if (trigger_buf_mem)
            {
                bufferA.data[3].written ^= (1 << 3);
                bufferB.data[6].read    ^= (1 << 6);
                buffer.data[7].dtime    ^= (1 << 2);
            }
            else if (trigger_hang)
            {
                while(1);
            }
#endif
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

    }

    // Reload stopwatch
    Stopwatch_TriggerReload();

    // Start stopwatch
    Stopwatch_TriggerStart();

    // Store DAC's next value
    nextValue = CTDAC0->CTDAC_VAL_NXT;

#if FAULT_EMULATION_EN
    static bool fault_latch = false;
    if (trigger_single_smpl && !fault_latch)
    {
        CTDAC0->CTDAC_VAL     ^= (1 << 11);
        CTDAC0->CTDAC_VAL_NXT ^= (1 << 11);
        fault_latch = true;
    }
    else if (trigger_fixed_smpl)
    {
        CTDAC0->CTDAC_VAL_NXT =
        CTDAC0->CTDAC_VAL     = fixed_sample;
    }
    else if (trigger_time && !fault_latch)
    {
        CyDelayUs(10);
        fault_latch = true;
    }

#endif

    // Start another conversion
    ADC_StartConvert();

}
//**************************************************************************************

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
//**************************************************************************************
