/*
 * Gruppe_27_Byggern.c
 *
 * Created: 01.09.2026 13:59:26
 * Author : andrksel
 */ 

#ifndef F_CPU
#define F_CPU 4915200UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "Protokoller/UART_driver.h"
#include "Utils/BitHandling.h"


void sqaureWaveFunc(){
	PORTB |= (1 << PB0);   // sett PB0 hih
	_delay_ms(250);   // styrer frekvensen
	PORTB ^= (1 << PB0);  // sett PB0 low
	_delay_ms(250);   // styrer frekvensen
}


int main(void)
{
	
	
	//Enable global interrupts
	sei();

	
	
	
	
	//Enable UART
	uart_init();
	
	
	DDRB |= (1 << PB0); //Enable Square Wave Function
	
	
	const char melding[] = "DATA funker fremdeles?\n";
	const char* ptrMelding = melding;
	while (1) 
    {
		sqaureWaveFunc();
		
		_delay_us(100);
		UCSR0B = UCSR0B | (1u << UDRIE0);
		
		//uart_receive();
		
		//uart_print(ptrMelding); 
			
			
    }
}




//-------------------------------INFO----------------------------------------------


/*
	DDRB(RETNING) - Data Direction Register - velger om skal vaare input eller output
	
	PORTB() - Dobbel funksjon avhengig av DDRB ()
		Hvis DDB0 = 1 (output):
		PORTB0 bestemmer spenningsnivaaet du sender ut:

		PORTB |= (1 << PB0);   // PB0 settes HIGH (5V)
		PORTB &= ~(1 << PB0);  // PB0 settes LOW (0V)
		
		Hvis DDB0 = 0 (input)
		PORTB |= (1 << PB0);   // aktiverer intern pull-up paa PB0 (som input)
	
	PINB - kun for aa lese input-verdi
		Uansett om pinnen er satt som input eller output, kan du lese den faktiske elektriske tilstanden paa pinnen via PINB:
		if (PINB & (1 << PB0)) {
			// PB0 er fysisk HIGH akkurat naa
		}
*/

