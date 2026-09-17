
/*
 * UART_driver.h
 *
 * Created: 08.09.2026 10:08:21
 *  Author: andrksel
 */ 

#pragma once

#define F_CPU 4915200UL

#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "../Utils/BitHandling.h"

//uint8_t buffer_is_empty(void);
//uint8_t buffer_is_full(void);
//uint8_t buffer_put(char data);
//uint8_t buffer_get(char *data);

void UART_init(void);
void UART_sendChar(char data);
void UART_sendString(const char *string);