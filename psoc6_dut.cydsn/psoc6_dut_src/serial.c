#include "serial.h"

//====================== DEFINES ======================
#define MAX_STRING_SIZE 100
#define BUFFER_CHUNK_LEN 100

//================= GLOBAL VARIABLES ==================
extern BUFFER_DATA buffer;
extern REFERENCE_VALUES reference;

//===================== FUNCTIONS =====================

/**
* @brief  Integer to byte
* @param  buf_ptr  : buffer pointer to store the bytes
* @param  data     : integer
* @param  byte_len : number of bytes to represent data
* @retval : The number of bytes written
*/
static uint16_t int2byte(uint8_t *buf_ptr, uint32_t data, uint8_t byte_len)
{
    for (int j = 0; j < byte_len; j++)
    {
        buf_ptr[j] = (data >> ((byte_len-1)-j)*8) & 0xFF;
    } 
    return byte_len;
}
//****************************************************************************************************

/**
* @brief  Build data packet
* @param  pckt_ptr  : packet buffer pointer
* @param  sequence  : chunk offset
* @param  timestamp : monitoring time
* @retval : The packet size in bytes
*/
static uint16_t UART_build_pckt(uint8_t *pckt_ptr, uint16_t sequence, uint32_t timestamp)
{
    uint16_t byte_cnt = 0;
    
    // Header
    pckt_ptr[byte_cnt++] = 'D'; // DUT ID
    pckt_ptr[byte_cnt++] = 'P'; // Data packet ID
    byte_cnt += int2byte(pckt_ptr + byte_cnt, sequence, 2);
    byte_cnt += int2byte(pckt_ptr + byte_cnt, timestamp, 4);
    
    // Payload
    for(int i = 0; (i < BUFFER_CHUNK_LEN) && (i + sequence < MAX_BUFFER_DATA); i++)
    {      
        //pckt_ptr[byte_cnt++] = buffer.data[sequence + i].WR_fault + (buffer.data[sequence + i].SR_fault << 1);
        pckt_ptr[byte_cnt++] = buffer.data[sequence + i].fault_descriptor;
        byte_cnt += int2byte(pckt_ptr + byte_cnt, buffer.data[sequence + i].dtime, 2);
        byte_cnt += int2byte(pckt_ptr + byte_cnt, buffer.data[sequence + i].read, 2);
        byte_cnt += int2byte(pckt_ptr + byte_cnt, buffer.data[sequence + i].written, 2);
    }
    
    // Compute checksum
    uint32_t checksum = 0;
    for(int i = 0; i < byte_cnt; i++)
    {
        checksum += (i & 1) ? pckt_ptr[i] : (pckt_ptr[i] << 8); 
    }
    // Add carry bits
    checksum += (checksum >> 16) & 0xF;
    
    // One's complement
    checksum = ~checksum;
    
    byte_cnt += int2byte(pckt_ptr + byte_cnt, checksum, 2);
    
    return byte_cnt;
}
//****************************************************************************************************

/**
* @brief  Listen to host via UART
* @retval : true if received acknowledge, 0 otherwise
*/
/*static uint8_t listen_host(void)
{
    enum listen_fsm_st
    {
      FSM_IDLE_ST,
      FSM_READ_CMD_ST
    };
    
    static int fsm_st = FSM_IDLE_ST;
    uint8_t rx_byte = 0, ack_received = 0;
    
    if ( UART_GetNumInRxFifo() > 0 )
    {
        rx_byte = UART_Get();
        switch(fsm_st)
        {
            // Idle state
            case FSM_IDLE_ST:
                // Host ID
                if (rx_byte == 'H')
                {
                    fsm_st = FSM_READ_CMD_ST;
                }        
            break;
              
            // Read command state
            case FSM_READ_CMD_ST:
                // Received ID
                if (rx_byte == 'R')
                {
                    ack_received = 1;
                }
                fsm_st = FSM_IDLE_ST;            
            break;
            
                // Invalid state
            default:
                fsm_st = FSM_IDLE_ST;
            break;            
        }             
    }
    return ack_received;
}*/
//****************************************************************************************************

/**
* @brief  Send buffer via UART
* @retval : none
*/
void UART_send_buffer(void)
{
    enum uart_fsm_st
    {
      FSM_IDLE_ST,
      FSM_SEND_CHUNK_ST,
      FSM_WAIT_ACK_ST
    };

    uint8_t uart_pckt[1024] = {0};
    uint8_t exit_flag = 0;
    uint16_t sequence = 0,
             pckt_len = 0;
             //timeout_cnt = 0;
    
    uint32_t timestamp = Timestamp_Timer_GetCounter();
    
    int fsm_st = FSM_IDLE_ST;
    
    while(!exit_flag)
    {
        switch(fsm_st)
        {
            // Idle state
            case FSM_IDLE_ST:
                //Stopwatch_TriggerStop();
                //timeout_cnt = 0;
                if (sequence < MAX_BUFFER_DATA)
                {
                    pckt_len = UART_build_pckt(uart_pckt, sequence, timestamp);
                    sequence += BUFFER_CHUNK_LEN;
                    fsm_st = FSM_SEND_CHUNK_ST; 
                }
                else
                {
                    exit_flag = 1;                    
                }            
                
            break;
            
            // Send new packet
            case FSM_SEND_CHUNK_ST:
                UART_PutArrayBlocking(uart_pckt, pckt_len); 
                while(!UART_IsTxComplete());
                
                // Starts timeout counter
                //Stopwatch_TriggerReload();
                //Stopwatch_TriggerStart();
                //fsm_st = FSM_WAIT_ACK_ST;
                fsm_st = FSM_IDLE_ST;
            break;
                
            // Wait acknowledge packet
            /*case FSM_WAIT_ACK_ST:
                
                // 250ms timeout
                while (  Stopwatch_GetCounter() <= 250000UL )
                {
                    if ( listen_host() )
                    {
                        fsm_st = FSM_IDLE_ST; 
                        break;
                    }
                    CyDelayUs(10);
                }
                // Retry to send packet 3 more times
                if (timeout_cnt < 3)
                {
                    timeout_cnt++;
                    fsm_st = FSM_SEND_CHUNK_ST;
                }
                else
                {
                    fsm_st = FSM_IDLE_ST;
                }                
            break;
            */
            // Invalid state
            default:
                fsm_st = FSM_IDLE_ST;
            break;
        };
    }  
}
//****************************************************************************************************

/**
* @brief  Dump to serial port reference values
* @retval : none
*/
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
//****************************************************************************************************

/**
* @brief  Dump to serial port buffer
* @retval : none
*/
static void UART_print_buffer(void)
{
    char string[MAX_STRING_SIZE] = {0};
    int16_t aux = 0;
    
    for(int i=0; i < MAX_BUFFER_DATA; i++)
    {
        if (i < MAX_BUFFER_DATA - 1)
        {
            aux = abs(buffer.data[i+1].read - buffer.data[i].read);
        }
        sprintf
        (
            string, "%4d,%4d,%3d,%3d,%3d,\t%d\r\n",
            buffer.data[i].read, 
            buffer.data[i].written, 
            abs( buffer.data[i].read-buffer.data[i].written ),
            aux,
            buffer.data[i].dtime,
            buffer.data[i].fault_descriptor
            //buffer.data[i].WR_fault + (buffer.data[i].SR_fault << 1)
        );

        UART_PutString(string); 
        while(!UART_IsTxComplete());
    }
}
//****************************************************************************************************

/**
* @brief  Dump to serial port data
* @param  print_header  : print header flag
* @param  print_ref     : print reference values flag
* @param  print_buffer  : print buffer flag
* @retval : none
*/
void UART_print_data(uint8_t print_header, uint8_t print_ref, uint8_t print_buffer) 
{

    char string[MAX_STRING_SIZE] = {0}; 
    
    // reference
    if(print_ref)
    {
        UART_print_ref();
    }
    
    // header
    if (print_header)
    {
        UART_PutString("dt \t ADC \t DAC \t deltaWR \t fault\r\n");
        while(!UART_IsTxComplete()); 
        
        sprintf(string, "fault count: SR-%i \t WR-%i  \r\n",buffer.SRfaultIndex, buffer.WRfaultIndex);
        UART_PutString(string);
        while(!UART_IsTxComplete());
    }   
    
    // buffer
    if (print_buffer)
    {
        UART_print_buffer();   
    }   

}
//****************************************************************************************************
