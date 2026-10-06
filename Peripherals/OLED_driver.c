
#include "OLED_driver.h"
#include "../Communication/SPI_driver.h"


extern uint8_t readBYTE;
extern uint8_t writeBYTE;
extern SPI_CS_t ChipSelect;


void bob(){
    _delay_ms(10);
}

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
	readBYTE = SPI_Transfer(writeBYTE, SPI_ID_OLED, 0);
	_delay_ms(150);

	writeBYTE = 0xA4;
	readBYTE = SPI_Transfer(writeBYTE, SPI_ID_OLED, 0);	

	writeBYTE = 0xA6;
	readBYTE = SPI_Transfer(writeBYTE, SPI_ID_OLED, 0);	
	
	SPI_Transfer(0x00 ,SPI_ID_OLED, 0);  // Lower column
	SPI_Transfer(0x10 ,SPI_ID_OLED, 0);  // Higher column
	//SKRU AV LED
	// writeBYTE = 0xAE;
	// readBYTE = SPI_Transfer(writeBYTE, ChipSelect);	

}

void reset(){
	for (uint8_t j = 0; j < 8; j++) {
		SPI_Transfer(0xB0 | (j & 0x0F), SPI_ID_OLED, 0);          // Page 0
		for (uint8_t i = 0x00; i < 0x80; i += 1){
	    	SPI_Transfer(0x00, SPI_ID_OLED, 1);
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
	OLED_write_data(0b00011000);void Init_OLED(void){
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
	OLED_write_data(0b00011000);
	OLED_write_data(0b01111110);
	OLED_write_data(0b00111100);
	OLED_write_data(0b00011000);
}