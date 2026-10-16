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
#include "Timers/Timer.h"


void timer1_init(void){
	//Set Mode 4: CTC with TOP = OCR1A
	TCCR3B	|= (1<<WGM32);
	//Set compare match for 100 Hz (10ms)
	//OCR1A = 7679;
	OCR3A = 767;
}

ISR(TIMER3_COMPA_vect){
	//Gamle kode før debounce bug
	// //Timer External interrupt Disable
	// ETIMSK &= ~(1<<OCIE3A);
	
	// // Clear pending INT0 flag caused by bouncing
	// GIFR |= (1 << INTF0);
	// //Button External interrupt Enable
	// GICR |= (1<<INT0);
	 
	// //Prescalar 64: Start Timer (CS11 = 1m CS10 = 1)
	// TCCR3B &= ~(1<<CS31);
	// TCCR3B &= ~(1 << CS30);

    static uint8_t releaseSamples = 0;

	//PD2 = JoyButton
    if (PIND & (1 << PD2)) {
        // Still pressed: keep INT0 disabled.
        releaseSamples = 0;
        return;
    }

    if (++releaseSamples < 3) {
        return;
    }

    releaseSamples = 0;

    // Stop Timer3 and disable its compare interrupt.
    TCCR3B &= ~((1 << CS32) | (1 << CS31) | (1 << CS30));
    ETIMSK &= ~(1 << OCIE3A);

    // Clear bounce interrupts before accepting another press.
    GIFR = (1 << INTF0);
    GICR |= (1 << INT0);
}
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
	timer1_init();
	InitJoystick(&JOYSTICK);
    UART_init();
	InitSRAM();
	Init_ADC();
	Init_SPI();

	Init_OLED();
	OLED_reset();
	//SRAM_test();


	///TESTING arrow printing on OLED
	OLED_print_arrow(4, 20);

	// for(uint8_t i = 0; i < 100;i++){
	// 	OLED_print_arrow(4, i);
	// }
		
	OLED_Menu_Home();

	while (1)
    {
        _delay_ms(1);
        OLED_Menu_Select(DIR_JOYSTICK);   // flytt markøren
        OLED_Menu_Enter();                // reager på knappetrykk
    }
}
		SRAMaddress.PTR = (volatile uint8_t*)0x1800;

		printf("BUTTON : %d\n",JoyBtnPressed);
		ReadAndScale(&JOYSTICK);	
		// JoyStickPos_Print(&JOYSTICK);
		DIR_JOYSTICK = JoyDirection(&JOYSTICK);
		// printf("DIRECTION: %d\n",DIR_JOYSTICK);
        
		sqaureWaveFuncPB0();

    }
}
