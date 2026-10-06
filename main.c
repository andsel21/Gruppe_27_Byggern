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

uint8_t readBYTE = 0x0;
uint8_t writeBYTE = 0x0;


//FUNKSJONELL KODE, MEN FLYTTET UT I EGEG C.file
/*
	
void Init_OLED(void){
	//PB2 = DISP D/C#
	DDRB |= (1 << PB2);
	PORTB &= ~(1 <<PB2);

	//PB1 = DISP_RES
    //DDRB |= (1<<PB1); // PB1 som output
    //PORTB &= ~(1<<PB1);
    _delay_ms(10);
	// PORTB |= (1<<PB1);

	writeBYTE = 0xAF;
	readBYTE = SPI_Transfer(writeBYTE, ChipSelect, 0);
	_delay_ms(150);

	writeBYTE = 0xA4;
	readBYTE = SPI_Transfer(writeBYTE, ChipSelect, 0);	

	writeBYTE = 0xA6;
	readBYTE = SPI_Transfer(writeBYTE, ChipSelect, 0);	
	
	SPI_Transfer(0x00 ,ChipSelect, 0);  // Lower column
	SPI_Transfer(0x10 ,ChipSelect, 0);  // Higher column
	//SKRU AV LED
	// writeBYTE = 0xAE;
	// readBYTE = SPI_Transfer(writeBYTE, ChipSelect);	

}


void reset(){
	for (uint8_t j = 0; j < 8; j++) {
		SPI_Transfer(0xB0 | (j & 0x0F), ChipSelect, 0);          // Page 0
		for (uint8_t i = 0x00; i < 0x80; i += 1){
	    	SPI_Transfer(0x00, ChipSelect, 1);
			// _delay_ms(1000);
		}
	}	
}

void OLED_POS(uint8_t page, uint8_t column){
	// SPI_Transfer(0xB0 | (page & 0x0F), ChipSelect, 0);  // Page 0-7
	// SPI_Transfer(0x00 | column,ChipSelect, 0); // Column
	// SPI_Transfer(0x10 ,ChipSelect, 0);  // Higher column
	SPI_Transfer(0xB0 | (page & 0x07), SPI_ID_OLED, 0);
    SPI_Transfer(0x00 | (column & 0x0F), SPI_ID_OLED, 0);
    SPI_Transfer(0x10 | ((column >> 4) & 0x0F), SPI_ID_OLED, 0);
}



void OLED_write_data(uint8_t data){
	SPI_Transfer(data, SPI_ID_OLED, 1);
	
}

void OLED_print_arrow(uint8_t page, uint8_t column){
	OLED_POS(page, column);
	OLED_write_data(0b00011000);
	OLED_write_data(0b00011000);
	OLED_write_data(0b01111110);
	OLED_write_data(0b00111100);
	OLED_write_data(0b00011000);
}

*/

int main(void)
{
	struct MemoryReadWrite SRAMaddress = {
		.package = 0x0F,
		.package_size = 2,
		.PTR = (volatile uint8_t*)0x1800
	};
	
	// joystickINIT(&JOYSTICK);	
	
			
	uint8_t ArrayToHoldTestData[SRAMaddress.package_size];
	
    UART_init();

    sei(); // Enable global interrupts
	
	InitSRAM();
	Init_ADC();
	Init_SPI();

	Init_OLED();
	reset();
	OLED_print_arrow(4, 80);
	OLED_print_arrow(4, 80);
	OLED_print_arrow(4, 80);
	OLED_print_arrow(4, 80);
	OLED_print_arrow(4, 80);
	OLED_print_arrow(4, 80);


	for(uint8_t i = 0; i < 100;i++){
	OLED_print_arrow(4, i);
	}




	
	//SRAM_test();

	while (1)
    {
		_delay_ms(1);


		
		SRAMaddress.PTR = (volatile uint8_t*)0x1800;
		


		// readBYTE =  SPI_Transfer(writeBYTE, ChipSelect);
		
		printf("SPI WRITE BYTE: %d  ",writeBYTE);
		printf("SPI READ BYTE: %d\n",readBYTE);
		
		//
		//
		//printf("BUTTON : %d\n",JoyBtnPressed);
//
		// ReadAndScale(&JOYSTICK);
		
		// JoyStickPos_Print(&JOYSTICK);
//
		//DIR_JOYSTICK = JoyDirection(&JOYSTICK);
		//printf("DIRECTION: %d\n",DIR_JOYSTICK);
		
       
        
		sqaureWaveFuncPB0();

		////SRAMaddress.PTR = 0x1800;
		//WriteSRAM(SRAMaddress);
		////SRAMaddress.PTR = 0x1800;
		//ReadSRAM(SRAMaddress, ArrayToHoldTestData);
		//
		// SRAM_test();

		//_delay_ms(250);

    }
}
