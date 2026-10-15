
#include "OLED_driver.h"
#include "../Communication/SPI_driver.h"
#include "../fonts.h"
#include "../Utils/PinStructure.h"
#include "../Utils/bithandling.h"

extern SPI_CS_t ChipSelect;

void Init_OLED(void){
	//PB2 = DISP D/C#
	*PINS.B_Config |= (1 << PB2_D_C); //Setter PB2 som output for OLED D/C#
	SET_BIT(*PINS.B_Config, PB2_D_C);

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



static const uint8_t menu_line[] = {1, 3, 5, 7};
uint8_t menu_lineSize = sizeof(menu_line) / sizeof(menu_line[0]);
static const uint8_t play_line[] = {3, 7};
uint8_t play_lineSize = sizeof(play_line) / sizeof(play_line[0]);
static const uint8_t gamemode_line[] = {3, 7};
uint8_t gamemode_lineSize = sizeof(gamemode_line) / sizeof(gamemode_line[0]);
static const uint8_t tutorial_line[] = {3, 7};
uint8_t tutorial_lineSize = sizeof(tutorial_line) / sizeof(tutorial_line[0]);
static const uint8_t calibrate_line[] = {3, 7};
uint8_t calibrate_lineSize = sizeof(calibrate_line) / sizeof(calibrate_line[0]);
static uint8_t indeks = 0;
static Direction_t old_direction = NEUTRAL;
extern volatile uint8_t JoyBtnPressed; // Declare the external variable
uint8_t aktiverte_line = play_line || gamemode_line || tutorial_line || calibrate_line;



void OLED_Menu_Pointer(uint8_t indeks) {			
    for (uint8_t i = 0; i < menu_lineSize; i++) {
        OLED_POS(menu_line[i], 0);
        OLED_print_char(i == indeks ? '>' : ' ');
	}
}

void OLED_Play_Pointer(uint8_t indeks) {
	for (uint8_t i = 0; i < play_lineSize; i++) {
		OLED_POS(play_line[i], 0);
		OLED_print_char(i == indeks ? '>' : ' ');
	}
}

void OLED_GameMode_Pointer(uint8_t indeks) {
	for (uint8_t i = 0; i < gamemode_lineSize; i++) {
		OLED_POS(gamemode_line[i], 0);
		OLED_print_char(i == indeks ? '>' : ' ');
	}
}

void OLED_Tutorial_Pointer(uint8_t indeks) {
	for (uint8_t i = 0; i < tutorial_lineSize; i++) {
		OLED_POS(tutorial_line[i], 0);
		OLED_print_char(i == indeks ? '>' : ' ');
	}
}

void OLED_Calibrate_Pointer(uint8_t indeks) {
	for (uint8_t i = 0; i < calibrate_lineSize; i++) {
		OLED_POS(calibrate_line[i], 0);
		OLED_print_char(i == indeks ? '>' : ' ');
	}
}

void OLED_Menu_Home(){
	menu_state = STATE_HOME;

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
	// OLED_Menu_Pointer(0);
}

	
void OLED_Play(){ //Play skjerm
	menu_state = STATE_PLAY;
	OLED_reset();
	OLED_POS(3, 20);
	OLED_print("Playing");
	OLED_POS(7, 20);
	OLED_print("Return");
	// OLED_Play_Pointer(3);
}

	
void OLED_GameMode(){ //Game mode skjerm
	menu_state = STATE_GAMEMODE;
	// aktiverte_line = gamemode_line;
	OLED_reset();
	OLED_POS(3, 20);
	OLED_print("Game mode");
	OLED_POS(7, 20);
	OLED_print("Return");
	// OLED_GameMode_Pointer();
}


void OLED_Tutorial(){ //Tutorial skjerm
	menu_state = STATE_TUTORIAL;
	// aktiverte_line = tutorial_line;	
	OLED_reset();
	OLED_POS(3, 20);
	OLED_print("Tutorial");
	OLED_POS(7, 20);
	OLED_print("Return");
	// OLED_Tutorial_Pointer();
}


void OLED_Calibrate(){ //Calibrate skjerm
	menu_state = STATE_CALIBRATE;
	// aktiverte_line = calibrate_line;
	OLED_reset();
	OLED_POS(3, 20);
	OLED_print("Calibrate");
	OLED_POS(7, 20);
	OLED_print("Return");
	// OLED_Calibrate_Pointer();
}

void OLED_Menu_Select(Direction_t direction) {
	menu_state = STATE_HOME;
	// aktiverte_line = menu_line;
	if (direction != old_direction) {            
            if (direction == DOWN && indeks < menu_lineSize - 1) {
                indeks++;
                OLED_Menu_Pointer(indeks);
            } else if (direction == UP && indeks > 0) {
                indeks--;
                OLED_Menu_Pointer(indeks);
            }
        } 
    old_direction = direction;         
}

void OLED_Play_Select(Direction_t direction) {
	menu_state = STATE_PLAY;
	//aktiverte_line = play_line;
	if (direction != old_direction) {            
			if (direction == DOWN && indeks < play_lineSize - 1) {
				indeks++;
				OLED_Play_Pointer(indeks);
			} else if (direction == UP && indeks > 0) {
				indeks--;
				OLED_Play_Pointer(indeks);
			}
		} 
	old_direction = direction;         
}

void OLED_GameMode_Select(Direction_t direction) {
	menu_state = STATE_GAMEMODE;
	// aktiverte_line = gamemode_line;
	if (direction != old_direction) {            
			if (direction == DOWN && indeks < gamemode_lineSize - 1) {
				indeks++;
				OLED_GameMode_Pointer(indeks);
			} else if (direction == UP && indeks > 0) {
				indeks--;
				OLED_GameMode_Pointer(indeks);
			}
		} 
	old_direction = direction;         
}

void OLED_Tutorial_Select(Direction_t direction) {
	menu_state = STATE_TUTORIAL;
	// aktiverte_line = tutorial_line;
	if (direction != old_direction) {            
			if (direction == DOWN && indeks < tutorial_lineSize - 1) {
				indeks++;
				OLED_Tutorial_Pointer(indeks);
			} else if (direction == UP && indeks > 0) {
				indeks--;
				OLED_Tutorial_Pointer(indeks);
			}
		} 
	old_direction = direction;         
}

void OLED_Calibrate_Select(Direction_t direction) {
	menu_state = STATE_CALIBRATE;
	//aktiverte_line = calibrate_line;
	if (direction != old_direction) {            
			if (direction == DOWN && indeks < calibrate_lineSize - 1) {
				indeks++;
				OLED_Calibrate_Pointer(indeks);
			} else if (direction == UP && indeks > 0) {
				indeks--;
				OLED_Calibrate_Pointer(indeks);
			}
		} 
	old_direction = direction;         
}

void OLED_Menu_Enter(){
	menu_state = STATE_HOME;
	
	if (JoyBtnPressed == 0) {            // ingen trykk, gjør ingenting
        return;
    }
    JoyBtnPressed = 0;                   // "bruk opp" trykket

	if (indeks == 0) {menu_state = STATE_PLAY; OLED_Play();
	} else if (indeks == 1) {menu_state = STATE_GAMEMODE; OLED_GameMode();
	} else if (indeks == 2) {menu_state = STATE_TUTORIAL; OLED_Tutorial();
	} else if (indeks == 3) {menu_state = STATE_CALIBRATE; OLED_Calibrate();
	}
	
}

void ReturnToHomeMenu() {
	menu_state = STATE_PLAY || menu_state == STATE_GAMEMODE || menu_state == STATE_TUTORIAL || menu_state == STATE_CALIBRATE;
	if (JoyBtnPressed == 0) {            // ingen trykk, gjør ingenting
        return;
    }
    JoyBtnPressed = 0;                   // "bruk opp" trykket

		if (indeks == 1) {menu_state = STATE_HOME; OLED_Menu_Home();
		}
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