
#include "UART_driver.h"


/* =========================================================
   UART SETTINGS
   ========================================================= */

#define BAUD        9600UL
#define UBRR_VALUE  ((F_CPU / (16UL * BAUD)) - 1)

#define BUFFER_SIZE 16


/* =========================================================
   TX RING BUFFER
   ========================================================= */

volatile char tx_buffer[BUFFER_SIZE];

volatile uint8_t tx_head = 0;
volatile uint8_t tx_tail = 0;


/* =========================================================
   PRIORITY CHARACTER
   ========================================================= */

/*
 * If a character is received from the PC, we store it here.
 * The UDRE interrupt will send this BEFORE the normal
 * characters waiting in the ring buffer.
 */

volatile char priority_char;
volatile uint8_t priority_pending = 0;


/* =========================================================
   RING BUFFER FUNCTIONS
   ========================================================= */

uint8_t buffer_is_empty(void)
{
    return (tx_head == tx_tail);
}


uint8_t buffer_is_full(void)
{
    return ((tx_head + 1) % BUFFER_SIZE == tx_tail);
}


uint8_t buffer_put(char data)
{
    uint8_t next = (tx_head + 1) % BUFFER_SIZE;

    /* Buffer full */
    if (next == tx_tail)
    {
        return 0;
    }

    tx_buffer[tx_head] = data;
    tx_head = next;

    return 1;
}


uint8_t buffer_get(char *data)
{
    /* Buffer empty */
    if (tx_head == tx_tail)
    {
        return 0;
    }

    *data = tx_buffer[tx_tail];

    tx_tail = (tx_tail + 1) % BUFFER_SIZE;

    return 1;
}


/* =========================================================
   UART INITIALIZATION
   ========================================================= */

void UART_init(void)
{
    /*
     * F_CPU = 4.9152 MHz
     * Baud   = 9600
     *
     * UBRR = F_CPU / (16 * BAUD) - 1
     *
     *      = 4915200 / (16 * 9600) - 1
     *      = 31
     */

    UBRR0H = (uint8_t)(UBRR_VALUE >> 8);
    UBRR0L = (uint8_t)UBRR_VALUE;


    /*
     * RXEN0  = Receiver Enable
     * TXEN0  = Transmitter Enable
     * RXCIE0 = Receive Complete Interrupt Enable
     *
     * RXCIE0 is NEW.
     *
     * It means that when a character arrives from
     * the computer, an interrupt will occur.
     */

    UCSR0B =
        (1 << RXEN0)  |
        (1 << TXEN0)  |
        (1 << RXCIE0);


    /*
     * Asynchronous
     * 8 data bits
     * No parity
     * 1 stop bit
     */

    UCSR0C =
        (1 << URSEL0) |
        (1 << UCSZ01) |
        (1 << UCSZ00);
}


/* =========================================================
   SEND ONE CHARACTER
   ========================================================= */

void UART_sendChar(char data)
{
    if (buffer_put(data))
    {
        /*
         * Enable Data Register Empty interrupt.
         */

        UCSR0B |= (1 << UDRIE0);
    }
}


/* =========================================================
   SEND STRING
   ========================================================= */

void UART_sendString(const char *string)
{
    while (*string != '\0')
    {
        while (buffer_is_full())
        {
			
        }

        UART_sendChar(*string);

        string++;
    }
}


/* =========================================================
   RECEIVE COMPLETE INTERRUPT
   ========================================================= */

ISR(USART0_RXC_vect)
{
    /*
     * Read received character from USART.
     */

    priority_char = UDR0;


    /*
     * Tell the TX interrupt that we have a priority
     * character waiting.
     */

    priority_pending = 1;


    /*
     * Make sure the UDRE interrupt is enabled.
     *
     * This will cause our transmit interrupt to run
     * when UDR0 is ready for another character.
     */

    UCSR0B |= (1 << UDRIE0);
}


/* =========================================================
   USART DATA REGISTER EMPTY INTERRUPT
   ========================================================= */

ISR(USART0_UDRE_vect)
{
    char data;

    if (priority_pending) //FIRST PRIORITY: Did we receive a character from the PC?
    {
        UDR0 = priority_char;

        priority_pending = 0;
    }
    else if (buffer_get(&data)) //SECOND PRIORITY:Send normal data from ring buffer
    {
        UDR0 = data;
    }
    else //Nothing to send
    {
        UCSR0B &= ~(1 << UDRIE0);
    }
}