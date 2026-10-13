
#include "OLED_driver.h"
#include "../Communication/SPI_driver.h"
#include "../fonts.h"



extern SPI_CS_t ChipSelect;

void Init_OLED(void){
	//PB2 = DISP D/C#
	DDRB |= (1 << PB2); //Setter PB2 som output for OLED D/C#
	DDRB |= (1 << PB3); //Setter PB3 som output for OLED CS
	PORTB &= ~(1 <<PB2); //Setter D/C# til 0 for command
	PORTB &= ~(1 <<PB3); //Setter CS til 0 for command

	OLED_write_command(0xA1); 
	OLED_write_command(0xC8); 

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

void OLED_goto_line(uint8_t line){
	SPI_Transfer(0xB0 | (line & 0x07), SPI_ID_OLED, 0); //Setter Page position
}

void OLED_goto_column(uint8_t column){
	SPI_Transfer(0x00 | (column & 0x0F), SPI_ID_OLED, 0); //Setter lower column position
	SPI_Transfer(0x10 | ((column >> 4) & 0x0F), SPI_ID_OLED, 0); //Setter upper column position
	// Lower column + upper*16 = plasseringsposisjon eks: (2x16)+5 = 37 = 0x25 ved 0x00 og 0x12
}

void OLED_POS(uint8_t page, uint8_t column){
	OLED_goto_line(page);
	OLED_goto_column(column);
}

void OLED_clear_line(uint8_t page){
	OLED_POS(page, 0);
	for (uint8_t i = 0x00; i < 0x80; i += 1){ // velger 128 kolonner
		OLED_write_data(0x00); //Skriver 0x00 til alle kolonner på den gitte pagen
	}
}

void OLED_home(){
	OLED_POS(0, 0); 
}

void OLED_write_data(uint8_t data){
	SPI_Transfer(data, SPI_ID_OLED, 1);
}

void OLED_write_command(uint8_t command){
	SPI_Transfer(command, SPI_ID_OLED, 0);
}

void OLED_print_char(char c){
    for (uint8_t i = 0; i < 8; i++) {
        OLED_write_data(pgm_read_byte(&font8[c - 32][i]));
    }
}

void OLED_print(char* str){
	while(*str){
		OLED_print_char(*str);
		str++;
	}
}

void OLED_Menu_Home(){
	// Page 0: Titel
 	OLED_POS(0, 8); OLED_print("Ping-Pong Game");
	// Page 1: Cleard
	OLED_clear_line(1);
	// Page 2: Play
	OLED_POS(1, 8); OLED_print("Play");
	// Page 3: Game mode
	OLED_clear_line(2); OLED_POS(3, 8); OLED_print("Game mode");
	// Page 4: Cleard
	OLED_clear_line(4);
	// Page 6: Tutorial
	OLED_clear_line(5); OLED_POS(5, 8); OLED_print("Tutorial");
	// Page 7: Calibrate
	OLED_clear_line(7); OLED_POS(7, 8); OLED_print("Calibrate");
}
void OLED_Play(){ //Play skjerm
	OLED_reset();
	OLED_POS(4, 20);
	OLED_print("Playing");
}

void OLED_GameMode(){ //Game mode skjerm
	OLED_reset();
	OLED_POS(4, 20);
	OLED_print("Game mode");
}

void OLED_Tutorial(){ //Tutorial skjerm
	OLED_reset();
	OLED_POS(4, 20);
	OLED_print("Tutorial");
}

void OLED_Calibrate(){ //Calibrate skjerm
	OLED_reset();
	OLED_POS(4, 20);
	OLED_print("Calibrate");
}

static const uint8_t menu_linjer[] = {1, 3, 5, 7};  // Play, Game mode, Tutorial, Calibrate
uint8_t linjeSize = sizeof(menu_linjer) / sizeof(menu_linjer[0]); // Number of menu lines

static uint8_t indeks = 0; // Current menu index
static Direction_t old_direction = NEUTRAL;

void OLED_Menu_POINTER(uint8_t indeks) {
    for (uint8_t i = 0; i < linjeSize; i++) {
        OLED_POS(menu_linjer[i], 0);
        OLED_print_char(i == indeks ? '>' : ' ');
	}
}

void OLED_Menu_Select(Direction_t direction){
	if(direction != NEUTRAL && old_direction != direction){
		if(direction == DOWN && indeks < linjeSize - 1){
			indeks ++;
			OLED_Menu_POINTER(indeks);
		} else if(direction == UP && indeks > 0){
			indeks--;
			OLED_Menu_POINTER(indeks);
		}
	}
	old_direction = direction;
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