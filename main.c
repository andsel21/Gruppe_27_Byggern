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

/* =========================================================
   MAIN
   ========================================================= */

joystick_io_t JOYSTICK;
Direction_t DIR_JOYSTICK = NEUTRAL;
SPI_CS_t ChipSelect;

extern volatile uint8_t JoyBtnPressed;
extern Buttons boardbuttons;

void Print_to_Screen_Joystick(joystick_io_t *js){
		if(menu_state==STATE_CALIBRATE){
        char buffer[8];

        // X
        OLED_POS(3, 36);
        sprintf(buffer, "%d", js->scaled_x_axis);
        OLED_print(buffer);
        OLED_print("    ");

        // Y
        OLED_POS(4, 36);
        sprintf(buffer, "%d", js->scaled_y_axis);
        OLED_print(buffer);
        OLED_print("    ");
		}
		
}

int main(void)
{
	struct MemoryReadWrite SRAMaddress = {
		.package = 0x0F,
		.package_size = 2,
		.PTR = (volatile uint8_t*)0x1800
	};
	
	DDRB |= (1 << PB0_LED_PULSE);
			
	uint8_t ArrayToHoldTestData[SRAMaddress.package_size];


    sei(); // Enable global interrupts
	timer3_init();
	InitJoystick(&JOYSTICK);
    UART_init();
	InitSRAM();
	Init_ADC();
	Init_SPI();

	Init_OLED();
	OLED_reset();

	//SRAM_test();


	OLED_DrawMenu();

	

    // Husk: Button function ut av ISR, Lag global
	// LED Off leaves PB0 high if already inside square wave function
	// Menu can be interruoted. OLED_Select with interrpt changing menu State before complete
	// To Do: 
	//	Buttons 

	Board_LEDS(0,1);
	Board_LEDS(1,1);
	Board_LEDS(2,1);
	Board_LEDS(3,1);
	Board_LEDS(4,0);
	Board_LEDS(5,1);

	// Board_PWM_LEDS(0,100);
	// Board_PWM_LEDS(1,200);
	// Board_PWM_LEDS(2,50);
	// Board_PWM_LEDS(3,0);
	// Board_PWM_LEDS(4,50);
	// Board_PWM_LEDS(5,255);


	while (1)
    {
		_delay_ms(1);
		OLED_Select(DIR_JOYSTICK);
		OLED_Menu_Action();
		Print_to_Screen_Joystick(&JOYSTICK);
		SRAMaddress.PTR = (volatile uint8_t*)0x1800;

		//printf("BUTTON : %d\n",JoyBtnPressed);
		ReadAndScale(&JOYSTICK);	
		//JoyStickPos_Print(&JOYSTICK);

		DIR_JOYSTICK = JoyDirection(&JOYSTICK);
		// printf("DIRECTION: %d\n",DIR_JOYSTICK);
        
		sqaureWaveFuncPB0();

		Board_Buttons();
		printf("hei %d   nr: %d\n", boardbuttons.L2,boardbuttons.L3);
    }
}

