
#include "UART_driver.h"


/* =========================================================
   UART SETTINGS
   ========================================================= */

#define BAUD        9600UL
#define UBRR_VALUE  ((F_CPU / (16UL * BAUD)) - 1)

struct ring_buffer{
	uint8_t *buffer;
	uint8_t bufferSize;
	uint8_t head;
	uint8_t tail;
};



#define BUFFER_SIZE 16
static uint8_t buffer[BUFFER_SIZE];
static struct ring_buffer tx_buffer = {.buffer = buffer, 
									   .bufferSize = sizeof(buffer)};



/* =========================================================
   TX RING BUFFER
   ========================================================= */

//volatile char tx_buffer[BUFFER_SIZE];
//
//volatile uint8_t tx_head = 0;
//volatile uint8_t tx_tail = 0;


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

uint8_t buffer_is_empty(struct ring_buffer *rd)
{ return (rd->head == rd->tail);}


uint8_t buffer_is_full(struct ring_buffer *rd)
{
    //return ((tx_head + 1) % BUFFER_SIZE == tx_tail);
	//return (rd->head + 1 == rd->tail); //Same same? nei!!

    uint8_t next = rd->head + 1;

    if (next >= rd->bufferSize)
    {
	    next = 0;
    }

    return (next == rd->tail);
    
}


uint8_t buffer_put(char data, struct ring_buffer *rd)
{
    if (buffer_is_full(rd))
    { return 0;}

    rd->buffer[rd->head] = data;
    rd->head++;

    if (rd->head >= rd->bufferSize)
    { rd->head = 0; }

    return 1;
}


uint8_t buffer_get(char *data, struct ring_buffer *rd)
{
    /* Buffer empty */
    if (buffer_is_empty(rd))
    {return 0;}

    *data = rd->buffer[rd->tail];

    //tx_tail = (tx_tail + 1) % BUFFER_SIZE;
	rd->tail++;
	
	if(rd->tail >= rd->bufferSize){
		rd->tail = 0;
	}

    return 1;
}


//
//int UART_putchar(char c, FILE *stream)
//{
	///* Convert \n to \r\n for terminal */
	//if (c == '\n')
	//{
		//while (buffer_is_full(&tx_buffer))
		//{
		//}
//
		//UART_sendChar('\r');
	//}
//
	//while (buffer_is_full(&tx_buffer))
	//{
	//}
//
	//UART_sendChar(c);
//
	//return 0;
//}

int UART_putchar(char c, FILE *stream)
{
	if (c == '\n')
	{
		UART_sendChar('\r');
	}

	UART_sendChar(c);

	return 0;
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
		
		/* Connect printf to our UART */
		fdevopen(UART_putchar,NULL);
}


/* =========================================================
   SEND ONE CHARACTER
   ========================================================= */

//void UART_sendChar(char data)
//{
    //if (buffer_put(data, &tx_buffer))
    //{
        ///*
         //* Enable Data Register Empty interrupt.
         //*/
//
        //UCSR0B |= (1 << UDRIE0);
    //}
//}


void UART_sendChar(char c)
{
	while (buffer_is_full(&tx_buffer))
	{
	}

	buffer_put(c, &tx_buffer);

	// Enable UDRE interrupt
	UCSR0B |= (1 << UDRIE0);
}


/* =========================================================
   SEND STRING
   ========================================================= */

//void UART_sendString(const char *string)
//{
    //while (*string != '\0')
    //{
        //while (buffer_is_full(&tx_buffer))
        //{
			//
        //}
//
        //UART_sendChar(*string);
//
        //string++;
    //}
//}


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
    else if (buffer_get(&data,&tx_buffer)) //SECOND PRIORITY:Send normal data from ring buffer
    {
        UDR0 = data;
    }
    else //Nothing to send
    {
        UCSR0B &= ~(1 << UDRIE0);
    }
}