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

extern volatile uint8_t JoyBtnPressed;


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
		
		printf("BUTTON : %d\n",JoyBtnPressed);

		ReadAndScale(&JOYSTICK);
		
		JoyStickPos_Print(&JOYSTICK);

		DIR_JOYSTICK = JoyDirection(&JOYSTICK);
		printf("DIRECTION: %d\n",DIR_JOYSTICK);
		
        _delay_ms(100);
		
		sqaureWaveFuncPB0();

		////SRAMaddress.PTR = 0x1800;
		//WriteSRAM(SRAMaddress);
		////SRAMaddress.PTR = 0x1800;
		//ReadSRAM(SRAMaddress, ArrayToHoldTestData);
		//
		//SRAM_test();

		//_delay_ms(250);

    }
}
