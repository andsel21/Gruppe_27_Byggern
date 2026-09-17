#ifndef F_CPU
#define F_CPU 4915200UL
#endif

#ifndef F_CPU
#define F_CPU 4915200UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include "Protokoller/UART_driver.h"
#include "Utils/BitHandling.h"
#include "TEST_Functions/TEST_Functions.h"

/* =========================================================
   MAIN
   ========================================================= */

int main(void)
{
    UART_init();

    /*
     * Enable global interrupts.
     */

    sei();


    while (1)
    {
		
		sqaureWaveFuncPB0();
        UART_sendString("Hello world!\r\n");
    }
}