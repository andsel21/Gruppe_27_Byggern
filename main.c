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


/*
 * SPI.c
 *
 * Created: 03.10.2026 16:28:59
 *  Author: andrksel
 */ 

#include "SPI_driver.h"
#include <stdio.h>
//#include "../Utils/PIN_Alias.h"
#define PB2_D_C PB2
#define SPI_SS PB4
#define SPI_MOSI PB5
#define SPI_MISO PB6
#define SPI_SCK PB7

//Distributed Shift Register

//			MCU					 SLAVE
//	    Shift Register		Shift Register
//

//Clock Phase 
// CPHA: 0/1
// CPOL: 0/1

//MODE 0 : CPOL = 0 og CPHA = 0
//MODE 1 : CPOL = 0 og CPHA = 1
//MODE 2 : CPOL = 1 og CPHA = 0
//MODE 3 : CPOL = 1 og CPHA = 1

//SPI STATUS REGISTER: SPIF, WCOL, SPI2X

//SPI CONTROL REGISTER - SPCR [SPIE, SPE, DORD,MSTR,CPOL,CPHA,SPR1,SPR0]


void Init_SPI(void){
	//Sets MOSI,SCK and SS as outputs. MISO remains an input
	// DDRB |= (1 << SPI_MOSI) |(1 << SPI_MISO) | (1 << SPI_SCK) | (1 << SPI_SS);
	DDRB |= (1 << SPI_MOSI) | (1 << SPI_SCK) | (1 << SPI_SS);
	DDRB &= ~(1 << SPI_MISO);
	//SPE = 1 (Enable), MSTR = 1 (MASTER), SPR1:0=00 (F_CPU/4)
	//CPOL = 0, CPHA = 0 (MODE 0)
	SPCR = (1 << SPE) | (1 << MSTR);
	//SPSR: Ensure no double speed
	SPSR &= ~(1 << SPI2X);
	//Setting Clock-rate fosc/64
	SPCR &= ~(1<<SPR0);
	SPCR |= (1<<SPR1); 
}


uint8_t SPI_Transfer(uint8_t byte, SPI_CS_t CS, uint8_t D_C){ //byte = data to send, CS = which slave to select, D_C = Data/Command for OLED
	
	if(CS == SPI_ID_CONTROLLER){ //N�r man sender data ut p� MOSI, vil OLED returnere samme data p� MISO.
		PORTB |= (1 << PB3); // Deaktiverer OLED SLAVE cHIP
		PORTB |= (1<<PB1); //Settes OLED CS Høy for deaktiver
		PORTB &= ~(1<<PB4); //Aktiver Slave chip(IO)

		if(D_C){
			PORTB |= (1<<PB2_D_C); //D/C# PIN - DATA	
		}else{
			PORTB &= ~(1<<PB2_D_C); // D/C# PIN - COMMAND
		}
		SPDR = byte;
		while(!(SPSR & (1 << SPIF))); //Wait for transmission complete
		// PORTB |= (1<<PB4); // deaktiverer etter melding
		// if(SPDR != byte){
		// 	printf("DIFFERENCE!! \n"); //Kan lages som til error-funksjon`?
		// }
		
		return SPDR;
	}

	if(CS == SPI_ID_OLED){
		PORTB |= (1 << PB4); // Deaktiverer CONTROLLER SLAVE cHIP
		PORTB |= (1<<PB1); //Settes OLED CS høy for deaktiver
		PORTB &= ~(1<<PB3); //Aktiver Slave chip(DISP_CS)
		
		if(D_C){
			PORTB |= (1<<PB2_D_C); //D/C# PIN - DATA	
		}else{
			PORTB &= ~(1<<PB2_D_C); // D/C# PIN - COMMAND
		}
		SPDR = byte;
		while(!(SPSR & (1 << SPIF)));
		// PORTB |= (1<<PB3);  // Deaktivere etter melding
		// if(SPDR != byte){
		// 	printf("OLED!! \n"); //Kan lages som til error-funksjon`?
		// }
		return SPDR;
	}
	
	if(CS == SPI_ID_CAN){
		PORTB |= (1 << PB4); // Deaktiverer CONTROLLER SLAVE cHIP
		PORTB |= (1<<PB3); //Deaktiverer Slave chip(DISP_CS)
		PORTB &= ~(1<<PB1); //Settes OLED CS lav for aktiver
		
		if(D_C){
			PORTB |= (1<<PB2_D_C); //D/C# PIN - DATA	
		}else{
			PORTB &= ~(1<<PB2_D_C); // D/C# PIN - COMMAND
		}
		SPDR = byte;
		while(!(SPSR & (1 << SPIF)));
		//Må også deaktiver SS3, når den tid kjem
		//DO DIS
		return SPDR;
	}
	
	if(CS == SS3){
		//DO DIS
			return SPDR;
	}
	//NB : Chip must be selected prior to call
	//SPDR = byte;
	//while(!(SPSR & (1 << SPIF))){ //Wait for transmission complete
}

Buttons boardbuttons;
uint8_t CtnRightBtns;
uint8_t CtnLeftBtns;
uint8_t Navigation;

void Board_Buttons(){
	SPI_Transfer(0x04, SPI_ID_CONTROLLER, 0); 	// Send Command = D/C= 0
	_delay_us(40);
	CtnRightBtns = SPI_Transfer(0x00, SPI_ID_CONTROLLER, 0);
	CtnLeftBtns = SPI_Transfer(0x00, SPI_ID_CONTROLLER, 0);
	Navigation = SPI_Transfer(0x00, SPI_ID_CONTROLLER, 0);
	PORTB |= (1<<PB4);
	boardbuttons.right = CtnRightBtns;
	boardbuttons.left = CtnLeftBtns;
	boardbuttons.nav = Navigation;
}


void Board_LEDS(uint8_t LED, uint8_t State){
	// LED = 0x00 to 0x04
	// State = 0x00: off, 0x01: on
	SPI_Transfer(0x05, SPI_ID_CONTROLLER, 0);
	_delay_us(40);
	SPI_Transfer(LED, SPI_ID_CONTROLLER, 0);
	_delay_us(40);
	SPI_Transfer(State, SPI_ID_CONTROLLER, 0);
	_delay_us(40);
	PORTB |= (1<<PB4);
}


void Board_PWM_LEDS(uint8_t LED, uint8_t PWM){
	// LED = 0x00 to 0x05
	// State = 0x00: off, 0xFF: Fully on
	SPI_Transfer(0x06, SPI_ID_CONTROLLER, 0);
	_delay_us(40);
	SPI_Transfer(LED, SPI_ID_CONTROLLER, 0);
	_delay_us(40);
	SPI_Transfer(PWM, SPI_ID_CONTROLLER, 0);
	PORTB |= (1<<PB4);
}

