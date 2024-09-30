#ifndef SERIAL_H
#define SERIAL_H

//====================== INCLUDES =====================
#include "gheader.h"
#include "buffer.h"

//============== FUNCTIONS PROTOTYPES =================
void UART_print_data(uint8_t print_header, uint8_t print_ref, uint8_t print_buffer);
void UART_send_buffer(void);

#endif