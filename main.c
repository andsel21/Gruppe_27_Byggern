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
#include "Utils/PIN_Alias.h"
#include "Communication/CAN_driver.h"

joystick_io_t JOYSTICK;
Direction_t DIR_JOYSTICK = NEUTRAL;
SPI_CS_t ChipSelect;

extern volatile uint8_t JoyBtnPressed;
extern volatile uint8_t JoyBtnPressed_CAN;
extern Buttons boardbuttons;

int main(void)
{

	DDRB |= (1 << PB0_LED_PULSE);
	DDRB |= (1 << PB1_CAN_CS);
			
    sei(); // Enable global interrupts
	timer3_init();
	InitJoystick(&JOYSTICK);
    UART_init();
	InitSRAM();
	Init_ADC();
	Init_SPI();
	Init_OLED();
	OLED_reset();

	SRAM_test();

	OLED_DrawMenu();

	Board_LEDS(0,1);
	Board_LEDS(1,1);
	Board_LEDS(2,1);
	Board_LEDS(3,1);
	Board_LEDS(4,0);
	Board_LEDS(5,1);

	Init_CAN(1);


	while (1)
    {
		_delay_ms(1);
		OLED_Select(DIR_JOYSTICK);
		OLED_Menu_Action();
		Print_to_Screen_Joystick(&JOYSTICK);
		// SRAMaddress.PTR = (volatile uint8_t*)0x1800;
		ReadAndScale(&JOYSTICK);	

		DIR_JOYSTICK = JoyDirection(&JOYSTICK);
		sqaureWaveFuncPB0();

		// Init_CAN(1);
		// CAN_test(); 
		_delay_ms(10);
 		CAN_Send(0, 0x60, (uint8_t)(JOYSTICK.scaled_y_axis + 100));
		CAN_Send(0, 0xE0, (uint8_t)(JOYSTICK.scaled_x_axis + 100));
		if(JoyBtnPressed_CAN==1){
			JoyBtnPressed_CAN = 0;
			CAN_Send(0, 0xC0, (uint8_t)(1));

		}
		Board_Buttons();
		// printf("hei %d   nr: %d\n", boardbuttons.L2,boardbuttons.L3);
		
    }
}

