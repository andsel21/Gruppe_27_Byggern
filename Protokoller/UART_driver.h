
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
#include <stdio.h>
#include "../Utils/BitHandling.h"

int UART_getchar(FILE *stream);
int UART_putchar(char c, FILE *stream);
unsigned char UART_receive(void);
void UART_transmit(unsigned char data);
void UART_init(void);