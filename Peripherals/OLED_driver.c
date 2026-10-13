
#include "OLED_driver.h"
#include "../Communication/SPI_driver.h"



extern SPI_CS_t ChipSelect;

void Init_OLED(void){
	//PB2 = DISP D/C#
	DDRB |= (1 << PB2); //Setter PB2 som output for OLED D/C#
	DDRB |= (1 << PB3); //Setter PB3 som output for OLED CS
	PORTB &= ~(1 <<PB2); //Setter D/C# til 0 for command
	PORTB &= ~(1 <<PB3); //Setter CS til 0 for command

	OLED_write_command(0xAF); // Display ON normal Mode
	_delay_ms(150);
	OLED_write_command(0xA4); //Entire display ON, use RAM content: D7-0
	OLED_write_command(0xA6); //Set Display to normal, A7 for inverse display, BLACK OR WHITE
	
}

void OLED_reset(){ //Clear the display by writing 0x00 to all pixels
	for (uint8_t j = 0; j < 8; j++) {//Clear all 8 pages
		OLED_write_command(0xB0 | (j & 0x0F));
		for (uint8_t i = 0x00; i < 0x80; i += 1){ //Clear all 128 columns
			OLED_write_data(0x00);
		}
	}	
}

void OLED_POS(uint8_t page, uint8_t column){

	SPI_Transfer(0xB0 | (page & 0x07), SPI_ID_OLED, 0); //Setter Page position
	// Lower column + upper*16 = plasseringsposisjon eks: (2x16)+5 = 37 = 0x25 ved 0x00 og 0x12
	SPI_Transfer(0x00 | (column & 0x0F), SPI_ID_OLED, 0); //Setter lower column position
    SPI_Transfer(0x10 | ((column >> 4) & 0x0F), SPI_ID_OLED, 0); //Setter upper column position
}

void OLED_write_data(uint8_t data){
	SPI_Transfer(data, SPI_ID_OLED, 1);
}

void OLED_write_command(uint8_t command){
	SPI_Transfer(command, SPI_ID_OLED, 0);
}


/* =========================================================
   PRINT ARROW FUNCTION FOR FUN
   ========================================================= */
void OLED_print_arrow(uint8_t page, uint8_t column){
	OLED_POS(page, column);
	OLED_write_data(0b00011000);
	OLED_write_data(0b00011000);
	OLED_write_data(0b01111110);
	OLED_write_data(0b00111100);
	OLED_write_data(0b00011000);
}