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

void WriteTest(){

		CAN_Write(NULL,0x30, 1,1, 0);  //Addresse TXB0CTRL - Buffer0
 		CAN_Write(NULL,0x00, 1,0, 1);  //not pending transmission(must be clear to write)

		//HIGH part
		CAN_Write(NULL,0x31, 1,1, 0);  //TXBnSIDH
 		CAN_Write(NULL,0x0, 1,0, 1);  //ID HIGH PART

		//LOW part
		CAN_Write(NULL,0x32, 1,1, 0);  //TXBnSIDL
 		CAN_Write(NULL,0x20, 1,0, 1);  //ID LOW PART 0010 0000
				
		//data length
		CAN_Write(NULL,0x35, 1,1, 0);  // TXBnDLC
 		CAN_Write(NULL,0x01, 1,0, 1);  //Length 8 bytes

		CAN_Write(NULL,0x2B, 1,1, 0);  //CANINTE – INTERRUPT 
 		CAN_Write(NULL,0x0, 1,0, 1);  //Disable interrupts

		CAN_Write(NULL,0x36, 1,1, 0);  // TXBnDm - Transmitt buffer n Data Byte m
 		CAN_Write(NULL,0xF1, 1,0, 1);  // data

		CAN_Write(NULL,0x30, 1,1, 0);  //Addresse TXB0CTRL - Buffer0
 		CAN_Write(NULL,0x8, 1,0, 1);   //Pending transmission
	
		_delay_ms(100);

}

void CAN_Send(uint8_t AdrH, uint8_t AdrL, uint8_t data){

		//AdrL 3 siste bits endres på = 3første bits i adresse, max 7
		//AdrH alle bits brukes, bit 3-11

		CAN_Write(NULL,0x30, 1,1, 0);  //Addresse TXB0CTRL - Buffer0
 		CAN_Write(NULL,0x00, 1,0, 1);  //not pending transmission(must be clear to write)

		//HIGH part
		CAN_Write(NULL,0x31, 1,1, 0);  //TXBnSIDH
 		CAN_Write(NULL,AdrH, 1,0, 1);  //ID HIGH PART

		//LOW part
		CAN_Write(NULL,0x32, 1,1, 0);  //TXBnSIDL
 		CAN_Write(NULL,AdrL, 1,0, 1);  //ID LOW PART 0010 0000
				
		//data length
		CAN_Write(NULL,0x35, 1,1, 0);  // TXBnDLC
 		CAN_Write(NULL,0x01, 1,0, 1);  //Length 8 bytes

		CAN_Write(NULL,0x2B, 1,1, 0);  //CANINTE – INTERRUPT 
 		CAN_Write(NULL,0x0, 1,0, 1);  //Disable interrupts

		CAN_Write(NULL,0x36, 1,1, 0);  // TXBnDm - Transmitt buffer n Data Byte m
 		CAN_Write(NULL,data, 1,0, 1);  // data

		CAN_Write(NULL,0x30, 1,1, 0);  //Addresse TXB0CTRL - Buffer0
 		CAN_Write(NULL,0x8, 1,0, 1);   //Pending transmission
	
		_delay_ms(100);

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
		// _delay_ms(250);
		
		_delay_ms(1);
		OLED_Select(DIR_JOYSTICK);
		OLED_Menu_Action();
		Print_to_Screen_Joystick(&JOYSTICK);
		SRAMaddress.PTR = (volatile uint8_t*)0x1800;

		// printf("BUTTON : %d\n",JoyBtnPressed);
		ReadAndScale(&JOYSTICK);	
		//JoyStickPos_Print(&JOYSTICK);

		DIR_JOYSTICK = JoyDirection(&JOYSTICK);
		//printf("DIRECTION: %d\n",DIR_JOYSTICK);

		//if(DIR_JOYSTICK == UP){
		//	WriteTest();
		//	_delay_ms(500);
		//}
		sqaureWaveFuncPB0();

		// Init_CAN(1);
		// CAN_test(); 
		_delay_ms(10);
 		CAN_Send(0, 0x60, (uint8_t)(JOYSTICK.scaled_x_axis + 100));
		//CAN_Send(0, 0x40, (uint8_t)(JOYSTICK.scaled_y_axis + 100));
		//TXB0CTRL
		// - bit3: TXREQ = SET TO REQUEST MESSAGE TO BE TRANSMITTET
		//Denne må settes for sende data fra bufferen.

		Board_Buttons();
		// printf("hei %d   nr: %d\n", boardbuttons.L2,boardbuttons.L3);

		
    }
}


