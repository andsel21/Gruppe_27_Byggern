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
#include "Timers/Timers.h"

/* =========================================================
   MAIN
   ========================================================= */

joystick_io_t JOYSTICK;
Direction_t DIR_JOYSTICK = NEUTRAL;
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
	timer3_init();
	InitJoystick(&JOYSTICK);
    UART_init();
	InitSRAM();
	Init_ADC();
	Init_SPI();

	Init_OLED();
	OLED_clear();

	//SRAM_test();


	OLED_Menu_Open(STATE_HOME);

	while (1)
    {
		_delay_ms(1);
		SRAMaddress.PTR = (volatile uint8_t*)0x1800;

		printf("BUTTON : %d\n",JoyBtnPressed);
		ReadAndScale(&JOYSTICK);	
		// JoyStickPos_Print(&JOYSTICK);
		DIR_JOYSTICK = JoyDirection(&JOYSTICK);
		OLED_Menu_Move(DIR_JOYSTICK);
		// printf("DIRECTION: %d\n",DIR_JOYSTICK);
        
		sqaureWaveFuncPB0();

    }
}
