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

#define CAN_writeregister 0x02

/* =========================================================
   MAIN
   ========================================================= */

	//TO download do : make clean && make


joystick_io_t JOYSTICK;
Direction_t DIR_JOYSTICK = NEUTRAL;
SPI_CS_t ChipSelect;

extern volatile uint8_t JoyBtnPressed;
extern Buttons boardbuttons;
extern uint8_t *dummyPTR;

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
	DDRB |= (1 << PB1_CAN_CS);
			
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
	Init_CAN(1);


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

		// Init_CAN(1);
		// CAN_test(); 
		_delay_ms(10);

	    // SPI_Transfer(CAN_writeregister, SPI_ID_CAN, 0);
		// CAN_Write(dummyPTR, 0x20, 0);
    	// PORTB |= (1 << PB1_CAN_CS); //CS high

		CAN_Write(dummyPTR,0x30, 1,1, 0);  //Addresse TXB0CTRL - Buffer0
 		CAN_Write(dummyPTR,0x00, 1,0, 1);  //not pending transmission(must be clear to write)

		//HIGH part
		CAN_Write(dummyPTR,0x31, 1,1, 0);  //TXBnSIDH
 		CAN_Write(dummyPTR,0x0, 1,0, 1);  //ID HIGH PART

		//LOW part
		CAN_Write(dummyPTR,0x32, 1,1, 0);  //TXBnSIDL
 		CAN_Write(dummyPTR,0x20, 1,0, 1);  //ID LOW PART 0010 0000
				
		//data length
		CAN_Write(dummyPTR,0x35, 1,1, 0);  // TXBnDLC
 		CAN_Write(dummyPTR,0x01, 1,0, 1);  //Length 8 bits

		CAN_Write(dummyPTR,0x2B, 1,1, 0);  //CANINTE – INTERRUPT 
 		CAN_Write(dummyPTR,0x0, 1,0, 1);  //Disable interrupts

		CAN_Write(dummyPTR,0x36, 1,1, 0);  // TXBnDm - Transmitt buffer n Data Byte m
 		CAN_Write(dummyPTR,0xFF, 1,0, 1);  // data

		CAN_Write(dummyPTR,0x30, 1,1, 0);  //Addresse TXB0CTRL - Buffer0
 		CAN_Write(dummyPTR,0x8, 1,0, 1);   //Pending transmission
		_delay_ms(100);

		



		//TXB0CTRL
		// - bit3: TXREQ = SET TO REQUEST MESSAGE TO BE TRANSMITTET
		//Denne må settes for sende data fra bufferen.

		// CAN_SendTest();
		Board_Buttons();
		// printf("hei %d   nr: %d\n", boardbuttons.L2,boardbuttons.L3);
    }
}

