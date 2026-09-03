/*
 * Gruppe_27_Byggern.c
 *
 * Created: 01.09.2026 13:59:26
 * Author : andrksel
 */ 

#define F_CPU 4915200UL
#define BAUD 9600
#define UBRR_VALUE (F_CPU/(16UL*BAUD)-1)


#include <avr/io.h>
#include <util/delay.h>



//#define  SET_BIT(x,n)  ((x) |= (1u<<(n)))
//#define  CLEAR_BIT(x,n)  ((x) &= ~(1u<<(n)))
//#define  TOGGLE_BIT(x,n)  ((x) ^= (1u<<(n)))
//#define  CHECK_BIT(x,n)  ((x) >> (n)) & 1u)

void uart_init(void)
{
	// Set baud rate
	UBRR0H = (unsigned char)(UBRR_VALUE >> 8);
	UBRR0L = (unsigned char)UBRR_VALUE;
	
	// Enable receiver and transmitter
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);

	// Frame format: 8 data bits, 1 stop bit, no parity
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}


void uart_transmit(unsigned char data)
{
	// Wait for empty transmit buffer
	while (!(UCSR0A & (1 << UDRE0)));

	// Put data into buffer, sends the data
	UDR0 = data;
}

unsigned char uart_receive(void)
{
	// Wait for data to be received
	while (!(UCSR0A & (1 << RXC0)));

	// Get and return received data from buffer
	return UDR0;
}



void uart_print(const char *str)
{
	while (*str)
	{
		uart_transmit(*str++);
	}
}


/*
	DDRB(RETNING) - Data Direction Register - velger om skal være input eller output
	
	PORTB() - Dobbel funksjon avhengig av DDRB ()
		Hvis DDB0 = 1 (output):
		PORTB0 bestemmer spenningsnivået du sender ut:

		PORTB |= (1 << PB0);   // PB0 settes HIGH (5V)
		PORTB &= ~(1 << PB0);  // PB0 settes LOW (0V)
		
		Hvis DDB0 = 0 (input)
		PORTB |= (1 << PB0);   // aktiverer intern pull-up på PB0 (som input)
	
	PINB - kun for å lese input-verdi
		Uansett om pinnen er satt som input eller output, kan du lese den faktiske elektriske tilstanden på pinnen via PINB:
		if (PINB & (1 << PB0)) {
			// PB0 er fysisk HIGH akkurat nå
		}
		
*/


void sqaureWaveFunc(){
	PORTB |= (1 << PB0);   // sett PB0 høy
	_delay_ms(250);   // styrer frekvensen
	PORTB ^= (1 << PB0);  // sett PB0 lav
	_delay_ms(250);   // styrer frekvensen
}

//Test å forandre noke for git push

int main(void)
{
    /* Replace with your application code */
	
	DDRB |= (1 << PB0); 
	
	uart_init();
	
	const char melding[] = "DataWoho\n";
	const char* ptrMelding = melding;
	    
	while (1) 
    {
		sqaureWaveFunc();
		
		
		//uart_receive();
		
		uart_print(ptrMelding);
		
			
    }
}

