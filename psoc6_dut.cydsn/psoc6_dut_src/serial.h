/*******************************************************************************
* @filename : serial.h
* @brief    : Serial communication handling header
*
* MIT License
*
* Copyright (c) 2024 eduardofabbris
* See the LICENSE file for details.
********************************************************************************/
#ifndef SERIAL_H
#define SERIAL_H

/*********************************************************
* Includes
*********************************************************/
#include "gheader.h"
#include "buffer.h"

/*********************************************************
* Function Prototypes
*********************************************************/

void UART_print_data(uint8_t print_header, uint8_t print_ref, uint8_t print_buffer);
void UART_send_buffer(void);

#endif
