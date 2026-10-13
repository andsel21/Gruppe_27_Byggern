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
#include "Communication/SPI_driver.h"
#include "Peripherals/OLED_driver.h"


/* =========================================================
   MAIN
   ========================================================= */

joystick_io_t JOYSTICK;
Direction_t DIR_JOYSTICK;
SPI_CS_t ChipSelect;

extern volatile uint8_t JoyBtnPressed;

int main(void)
{
	struct MemoryReadWrite SRAMaddress = {
		.package = 0x0F,
		.package_size = 2,
		.PTR = (volatile uint8_t*)0x1800
	};
	
	
			
	uint8_t ArrayToHoldTestData[SRAMaddress.package_size];
	

    sei(); // Enable global interrupts
    UART_init();
	InitJoystick(&JOYSTICK);	
	InitSRAM();
	Init_ADC();
	Init_SPI();

	Init_OLED();
	OLED_reset();
	//SRAM_test();


	// ///TESTING arrow printing on OLED
	// OLED_print_arrow(4, 20);

	// for(uint8_t i = 0; i < 100;i++){
	// 	OLED_print_arrow(4, i);
	// }

	SRAMaddress.PTR = (volatile uint8_t*)0x1800;
	

	GetChar("HELLO WORLD");
		


	while (1)
    {
		_delay_ms(1);

		SRAMaddress.PTR = (volatile uint8_t*)0x1800;

		//printf("BUTTON : %d\n",JoyBtnPressed);
		// ReadAndScale(&JOYSTICK);	
		// JoyStickPos_Print(&JOYSTICK);
		// DIR_JOYSTICK = JoyDirection(&JOYSTICK);
		// printf("DIRECTION: %d\n",DIR_JOYSTICK);
        
		sqaureWaveFuncPB0();

    }
}
