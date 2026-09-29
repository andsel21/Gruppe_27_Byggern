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
#include "Peripherals/ADC.h"
#include "Peripherals/Joystick.h"


//Legger til tekst for git check

/* =========================================================
   MAIN
   ========================================================= */



int main(void)
{
	
	struct MemoryReadWrite SRAMaddress = {
		.package = 0x00,
		.package_size = 2,
		.PTR = 0x1800
	};
	
	
	volatile uint8_t *ext_ram = (uint8_t *) 0x1800;
	uint8_t write_errors;
	uint8_t retrieval_errors;
	SRAMaddress.package_size = 2;
	
	uint8_t adc_value;
			
	uint8_t ArrayToHoldTestData[SRAMaddress.package_size];
	
    UART_init();

    sei(); // Enable global interrupts
	
	InitSRAM();
	//Init_ADC();
	
	joystickINIT();
	//SRAM_test();

	
	while (1)
    {
		
		SRAMaddress.package = 0x0F;

		sqaureWaveFuncPB0();
		//SRAMaddress.PTR = 0x1800;
		WriteSRAM(SRAMaddress);
		//SRAMaddress.PTR = 0x1800;
		ReadSRAM(SRAMaddress, ArrayToHoldTestData);
		
		//SRAM_test();
	
////
		//uint8_t i = 0;
		//uint8_t randomtall = 0xFF;
		//for (i=0; i < 4; i++)
		//{
			//ext_ram[i] = randomtall;
			//uint8_t readValue = ext_ram[i];
			//
			//printf("readValue: %d\n",readValue);
		//}
		//_delay_ms(250);
		
		
		
	
		
        //UART_sendString("Hello world!\r\n"); <---> old code
		//printf("Hello World!\n");
		
		
		//adc_value = ADC_read_channel(0);
		//printf("Her kommer value: %d\n",adc_value);	
		

		//ext_ram = 0x1800;
		//for (i =0; i < 2; i++)
		//{
			//ext_ram[i] = randomtall;
			//uint8_t readValue = ext_ram[i];
			//
			//printf("readValue: %d\n",readValue);
		//}
				

    }
}
