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
     
    // Start bias
    Stopwatch_TriggerStart(); // starts timer
    ADC_StartConvert();       // starts ADC conversion         
    

    while(1);
}


//interrupt -> the function is called every time it finishes a conversion
void ADC_ISR_Callback(void)
{
#if PSOC_MONITOR_ON
    static uint32_t UART_aliveSignalCounter = 0;
#endif
    static uint16_t nextValue;
    static bool firstConv = true;

    //stop stopwatch
    Stopwatch_TriggerStop();
    
    
    //if buffer is not full yet
    if(buffer.dataIndex < MAX_BUFFER_DATA) {

        //first value is neglected 
        if(!firstConv) {
            //gets DAC value
            buffer.data[buffer.dataIndex].written = nextValue;     
            
            //gets ADC value
            buffer.data[buffer.dataIndex].read =  ADC_GetResult16(0);      

            //gets time
            uint32_t aux = Stopwatch_GetCounter();
            if (aux >= (1 << 16))
            {
                Cy_GPIO_Write(RED_LED_0_PORT, RED_LED_0_NUM, 0);                
            }
            
            buffer.data[buffer.dataIndex].dtime = aux;       
 
            buffer.dataIndex++;
        }  
        else {

            firstConv = false;    
        }
                
    }  
    else {
        //alive signal
#if PSOC_MONITOR_ON
        
        //Cy_GPIO_Write(ALIVE_SIGNAL_0_PORT, ALIVE_SIGNAL_0_NUM, 1);
        Cy_GPIO_Inv(ALIVE_SIGNAL_0_PORT, ALIVE_SIGNAL_0_NUM);
        
        //serial alive signal
        if(UART_aliveSignalCounter < 500){
            UART_aliveSignalCounter++;
        }
        else{
            //while(!UART_IsTxComplete());
            UART_PutString("DA");                           // UART DUT Alive
            Cy_GPIO_Inv(GREEN_LED_0_PORT, GREEN_LED_0_NUM); // Visual Alive           
            UART_aliveSignalCounter = 0;   
        }
#endif           
        if(reference.cycleIndex < MAX_REFERENCE_VALUES_CYCLE) {          
            getReferenceValues();
            #if DEBUG_CODE    
                UARTPrintData(); //print training data
            #endif 
            reference.cycleIndex++;        

        // If debug mode, only send first 5 buffers via UART
        } else if(buffer.cycleIndex < MAX_BUFFER_DATA_CYCLE || !DEBUG_CODE) {

            //if a error is found. -> i.e., sends all data only if a fault is detected to plot the curve
            if(verifyFaults() || DEBUG_CODE){
                
                //UARTPrintData();
                UART_send_buffer();

                
                // Reset visual warning    
                /*for(int i=0; i <2*3; i++){
                    Cy_GPIO_Inv(GREEN_LED_0_PORT, GREEN_LED_0_NUM);
                    CyDelay(250);   
                }
                */    
                //reset kit with watchdog
                WD_reset_Start();

            } 
            
            buffer.cycleIndex++;
          } 
        

        buffer.dataIndex = 0; 
        firstConv = true;
#if PSOC_MONITOR_ON
        //Cy_GPIO_Write(ALIVE_SIGNAL_0_PORT, ALIVE_SIGNAL_0_NUM, 0);
#endif
    }

    //stopwatch
    Stopwatch_TriggerReload();//reload 
    Stopwatch_TriggerStart();//starts again     
    
    //starts again another conversion
    nextValue = CTDAC0->CTDAC_VAL_NXT; //stores the next value in DAC
    
    //if(Cy_GPIO_Read(Button_0_PORT, Button_0_NUM)) nextValue ^= (1 << 5);
    ADC_StartConvert();

}


void initDevices(){

    // Enable global interrupts.
    __enable_irq();
    
    //UART
    UART_Start();

    //Timer
    Stopwatch_Init(&Stopwatch_config);
    Stopwatch_Enable();

    //DMA
    DMA1_Start(&triangularWaveLUT, (uint32_t *)&(CTDAC0->CTDAC_VAL_NXT));

    //DAC
    DAC_Start();  
    
    //ADC
    ADC_Start();   
    ADC_IRQ_Enable();    
}







