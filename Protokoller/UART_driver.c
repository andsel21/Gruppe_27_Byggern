
/*
 * UART_driver.c
 *
 * Created: 08.09.2026 10:08:04
 *  Author: andrksel
 */ 
#include <avr/io.h>
#include "UART_driver.h"



void uart_init(void)
{
	// Set baud rate
	UBRR0H = (unsigned char)(UBRR_VALUE >> 8);
	UBRR0L = (unsigned char)UBRR_VALUE;
	
	// Enable receiver and transmitter
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);

	// Frame format: 8 data bits, 1 stop bit, no parity
	//UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
	// Frame format: 8 data bits, 1 stop bit, no parity
	UCSR0C = (1<<URSEL0)|(1<<USBS0)|(3<<UCSZ00);
}

unsigned char uart_receive(void)
{
	// Wait for data to be received
	while (!(UCSR0A & (1 << RXC0)));

	// Get and return received data from buffer
	return UDR0;
}

void uart_transmit(unsigned char data)
{
	// Wait for empty transmit buffer
	while (!(UCSR0A & (1 << UDRE0)));

	// Put data into buffer, sends the data
	UDR0 = data;
}


void uart_print(const char *str)
{
	while (*str)
	{
		uart_transmit(*str++); //Bruker tegnet str peker på deretter oeker pekeren til neste tegn.
	}
}

//fdevopen(transmit function, receive function);
