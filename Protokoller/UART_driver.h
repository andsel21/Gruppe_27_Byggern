
/*
 * UART_driver.h
 *
 * Created: 08.09.2026 10:08:21
 *  Author: andrksel
 */ 

#pragma once

#define F_CPU 4915200UL
#define BAUD 9600
#define UBRR_VALUE ((F_CPU/(16UL*BAUD))-1)

#include <avr/io.h>



void uart_init(void);
unsigned char uart_receive(void);
void uart_print(const char *str);
void uart_transmit(unsigned char data);