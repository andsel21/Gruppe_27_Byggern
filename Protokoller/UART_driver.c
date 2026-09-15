
/*
 * UART_driver.c
 *
 * Created: 08.09.2026 10:08:04
 *  Author: andrksel
 */ 
#include "UART_driver.h"
#include "../Utils/BitHandling.h"


unsigned int counter = 0;
char melding[] = "Hello world!";
unsigned int size = sizeof(melding) / sizeof(melding[0]);



/* =========================================================
   UART INITIALISERING
   ========================================================= */

void UART_init(void)
{
    /* Sett baudrate */
    UBRR0H = (unsigned char)(UBRR_VALUE >> 8);
    UBRR0L = (unsigned char)UBRR_VALUE;

    /*
     * Enable Receiver og Transmitter.
     * Ingen interrupts brukes.
     */
    UCSR0B =
        (1u << RXEN0) |
        (1u << TXEN0);

    /* Formatering
     * URSEL0 = velg UCSR0C
     * 8 databits
     * No parity
     * 1 stopbit
     */
    UCSR0C =
        (1u << URSEL0) |
        (1u << UCSZ01) |
        (1u << UCSZ00);
}


/* =========================================================
   FUNKSJON FOR INPUT
   ========================================================= */

int UART_getchar(FILE *stream)
{
    return UART_receive();
}


/* =========================================================
   RECEIVE ÉN CHARACTER
   ========================================================= */

unsigned char UART_receive(void)
{
    /*
     * Vent til en character er mottatt.
     *
     * RXC0 = 0 -> ingen ny data
     * RXC0 = 1 -> data tilgjengelig
     */
    while (!(UCSR0A & (1u << RXC0)))
    {
        /* Vent */
    }

    return UDR0;
}

/* =========================================================
   FUNKSJON FOR PRINTF
   ========================================================= */

int UART_putchar(char c, FILE *stream)
{
    /*
     * Gjør \n om til \r\n slik at terminalen
     * får korrekt linjeskift.
     */
    if (c == '\n')
    {
        UART_transmit('\r');
    }

    UART_transmit((unsigned char)c);

    return 0;
}

/* =========================================================
   SEND ÉN CHARACTER
   ========================================================= */

void UART_transmit(unsigned char data)
{
    /*
     * Vent så lenge UDR0 IKKE er ledig.
     *
     * UDRE0 = 0 -> UDR0 opptatt
     * UDRE0 = 1 -> UDR0 ledig
     */
    while (!(UCSR0A & (1u << UDRE0)))
    {
        /* Vent */
    }

    /* Legg ny character i transmit-buffer */
    UDR0 = data;
}


