#ifndef F_CPU
#define F_CPU 4915200UL
#endif

#include <avr/io.h>
#include <stdio.h>
#include "Protokoller/UART_driver.h"
#include "Utils/BitHandling.h"

#define BAUD 9600UL
#define UBRR_VALUE ((F_CPU / (16UL * BAUD)) - 1)

//
//
///* =========================================================
   //UART INITIALISERING
   //========================================================= */
//
//void UART_init(void)
//{
    ///* Sett baudrate */
    //UBRR0H = (unsigned char)(UBRR_VALUE >> 8);
    //UBRR0L = (unsigned char)UBRR_VALUE;
//
    ///*
     //* Enable Receiver og Transmitter.
     //* Ingen interrupts brukes.
     //*/
    //UCSR0B =
        //(1u << RXEN0) |
        //(1u << TXEN0);
//
    ///*
     //* URSEL0 = velg UCSR0C
     //* 8 databits
     //* No parity
     //* 1 stopbit
     //*/
    //UCSR0C =
        //(1u << URSEL0) |
        //(1u << UCSZ01) |
        //(1u << UCSZ00);
//}
//
//
///* =========================================================
   //SEND ÉN CHARACTER
   //========================================================= */
//
//void UART_transmit(unsigned char data)
//{
    ///*
     //* Vent så lenge UDR0 IKKE er ledig.
     //*
     //* UDRE0 = 0 -> UDR0 opptatt
     //* UDRE0 = 1 -> UDR0 ledig
     //*/
    //while (!(UCSR0A & (1u << UDRE0)))
    //{
        ///* Vent */
    //}
//
    ///* Legg ny character i transmit-buffer */
    //UDR0 = data;
//}
//
//
///* =========================================================
   //RECEIVE ÉN CHARACTER
   //========================================================= */
//
//unsigned char UART_receive(void)
//{
    ///*
     //* Vent til en character er mottatt.
     //*
     //* RXC0 = 0 -> ingen ny data
     //* RXC0 = 1 -> data tilgjengelig
     //*/
    //while (!(UCSR0A & (1u << RXC0)))
    //{
        ///* Vent */
    //}
//
    //return UDR0;
//}
//
//
///* =========================================================
   //FUNKSJON FOR PRINTF
   //========================================================= */
//
//int UART_putchar(char c, FILE *stream)
//{
    ///*
     //* Gjør \n om til \r\n slik at terminalen
     //* får korrekt linjeskift.
     //*/
    //if (c == '\n')
    //{
        //UART_transmit('\r');
    //}
//
    //UART_transmit((unsigned char)c);
//
    //return 0;
//}
//
//
///* =========================================================
   //FUNKSJON FOR INPUT
   //========================================================= */
//
//int UART_getchar(FILE *stream)
//{
    //return UART_receive();
//}


/* =========================================================
   MAIN / TESTPROGRAM
   ========================================================= */
void sqaureWaveFunc(){
	PORTB |= (1 << PB0);   // sett PB0 hih
	_delay_ms(250);   // styrer frekvensen
	PORTB ^= (1 << PB0);  // sett PB0 low
	_delay_ms(250);   // styrer frekvensen
}



int main(void)
{
    unsigned char received;
    int value = 42;
	//DDRB |= (1 << PB0); //Enable Square Wave Function
	SET_BIT(DDRB,PB0); //Enable Square Wave Function

    UART_init();

    /*
     * Koble stdin/stdout til UART-driveren.
     */
    fdevopen(UART_putchar, UART_getchar);

    /*
     * Test printf
     */
    printf("UART test started\n");
    printf("Value = %d\n", value);
    printf("Type characters on the PC:\n");

    while (1)
    {
        /*
         * Vent på character fra PC.
         */
        received = UART_receive();

        /*
         * Send den samme tilbake til PC.
         */
        UART_transmit(received);
		
		//
		//sqaureWaveFunc();
		
    }


    return 0;
}