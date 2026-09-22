#ifndef F_CPU
#define F_CPU 4915200UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include "Communication/UART_driver.h"
#include "Utils/BitHandling.h"
#include "TEST_Functions/TEST_Functions.h"
#include "ExternalMemory/SRAM.h"
#include "ExternalMemory/ADC.h"


//Legger til tekst for git check

/* =========================================================
   MAIN
   ========================================================= */

int main(void)
{
    UART_init();

    sei(); // Enable global interrupts
	
	InitSRAM();
	Init_ADC();
	
	SRAM_test();
	
	uint8_t adc_value;
	while (1)
    {

		sqaureWaveFuncPB0();
        //UART_sendString("Hello world!\r\n"); <---> old code
		//printf("Hello World!\n");
		adc_value = ADC_read_channel(0);
		printf("Her kommer value: %d\n",adc_value);	
	
		
    }
}
