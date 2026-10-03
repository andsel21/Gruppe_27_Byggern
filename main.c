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


/* =========================================================
   MAIN
   ========================================================= */

joystick_io_t JOYSTICK;
Direction_t DIR_JOYSTICK;

int main(void)
{
	struct MemoryReadWrite SRAMaddress = {
		.package = 0x0F,
		.package_size = 2,
		.PTR = (volatile uint8_t*)0x1800
	};
	
	joystickINIT(&JOYSTICK);	
	
			
	uint8_t ArrayToHoldTestData[SRAMaddress.package_size];
	
    UART_init();

    sei(); // Enable global interrupts
	
	InitSRAM();
	Init_ADC();
	
	//SRAM_test();

	while (1)
    {
		SRAMaddress.PTR = (volatile uint8_t*)0x1800;

		JOYSTICK = ReadAndScale();
		
		JoyStickPos_Print(&JOYSTICK);

		DIR_JOYSTICK = JoyDirection(&JOYSTICK);
		
        _delay_ms(100);
		
		sqaureWaveFuncPB0();

		////SRAMaddress.PTR = 0x1800;
		//WriteSRAM(SRAMaddress);
		////SRAMaddress.PTR = 0x1800;
		//ReadSRAM(SRAMaddress, ArrayToHoldTestData);
		//
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
		_delay_ms(250);
		
		
		
	
		
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
