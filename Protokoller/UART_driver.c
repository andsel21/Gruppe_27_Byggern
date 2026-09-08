
/*
 * UART_driver.c
 *
 * Created: 08.09.2026 10:08:04
 *  Author: andrksel
 */ 
#include "UART_driver.h"
#include "../Utils/BitHandling.h"


unsigned int counter = 0;
char Langmelding[] = "DDDDDDDTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTBBBBBB";
unsigned int size = sizeof(Langmelding) / sizeof(Langmelding[0]);

//char* packagePtr = Langmelding;

//RECEIVE DATA
ISR(USART0_RXC_vect) 
{
	unsigned char data = UDR0; //Received byte
	if (data == 'a'){ uart_printf(data);
	}
}

//TRANSMIT DATA
ISR(USART0_UDRE_vect) { //Skal håndtere noke
	UDR0 = Langmelding[counter];
	counter++;
	
	if(counter >= size){ //Full? Forlat Interrupt!
		CLEAR_BIT(UCSR0B,UDRIE0);
		counter = 0;
	}
}

void writeOutput(){
	
		//packagePtr = Langmelding;
		CLEAR_BIT(UCSR0B,UDRIE0);
}


void uart_init(void)
{
	// Set baud rate
	UBRR0H = (unsigned char)(UBRR_VALUE >> 8);
	UBRR0L = (unsigned char)UBRR_VALUE;
	
	
	//Page 186/187
	// Enable receiver and transmitter
	UCSR0B = (1u << RXEN0) | (1u << TXEN0);
	
	//Enable Local Read interrupts
	UCSR0B = UCSR0B | (1u << RXCIE0);
	UCSR0A = UCSR0A | (1u << RXC0);
	
	//Enable local Write interrupts
	//UCSR0B = UCSR0B | (1u << UDRIE0);
	UCSR0A = UCSR0A | (1u << UDRE0);

	// Frame format: 8 data bits, 1 stop bit, no parity
	UCSR0C = (1u<<URSEL0) | (1u << UCSZ01) | (1u << UCSZ00);
			
}

unsigned char uart_receive(void)
{
	// Wait for data to be received
	while ((UCSR0A & (1u << RXC0)));

	// Get and return received data from buffer
	return UDR0;
}


void uart_printf(char c){
	UDR0 = c;
}


void uart_transmit(unsigned char data)
{
	// Wait for empty transmit buffer
	while (!(UCSR0A & (1u << UDRE0)));

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
