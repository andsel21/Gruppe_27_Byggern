
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

//Meny design
#define ANTALL(arr) (sizeof(arr) / sizeof(arr[0]))

static const uint8_t menu_line[]      = {2, 4, 6, 7};  // Play, Game mode, Tutorial, Calibrate
static const uint8_t play_line[]      = {3, 7};        // Start game, Back to menu
static const uint8_t gamemode_line[]  = {3, 7};        // Single player, Back to menu
static const uint8_t tutorial_line[]  = {3, 7};        // Start tutorial, Back to menu
static const uint8_t calibrate_line[] = {3, 7};        // Calibrate now, Back to menu

static const uint8_t* aktiv_line = menu_line;          // menyen som vises nå
static uint8_t aktiv_lineSize = ANTALL(menu_line);
static uint8_t indeks = 0;                             // markert valg i aktiv meny
static uint8_t home_indeks = 0;                        // husker plassen i hovedmenyen

static MenuState_t menu_state = STATE_HOME;
static Direction_t old_direction = NEUTRAL;
extern volatile uint8_t JoyBtnPressed;

static void OLED_Pointer() {
	for (uint8_t i = 0; i < aktiv_lineSize; i++) {
		OLED_POS(aktiv_line[i], 0);
		OLED_print_char(i == indeks ? '>' : ' ');
	}
}

static void set_aktiv_meny(MenuState_t state, const uint8_t* lines, uint8_t size, uint8_t start) {
	menu_state     = state;
	aktiv_line     = lines;
	aktiv_lineSize = size;
	indeks         = start;
	OLED_reset();
}

//Skjermer
void OLED_Menu_Home() {
	set_aktiv_meny(STATE_HOME, menu_line, ANTALL(menu_line), home_indeks);
	OLED_POS(0, 8); OLED_print("Ping-Pong Game");
	OLED_POS(2, 8); OLED_print("Play");
	OLED_POS(4, 8); OLED_print("Game mode");
	OLED_POS(6, 8); OLED_print("Tutorial");
	OLED_POS(7, 8); OLED_print("Calibrate");
	OLED_Pointer();
}

void OLED_Play() {
	set_aktiv_meny(STATE_PLAY, play_line, ANTALL(play_line), 0);
	OLED_POS(0, 8); OLED_print("Play");
	OLED_POS(3, 8); OLED_print("Start game");
	OLED_POS(7, 8); OLED_print("Back to menu");
	OLED_Pointer();
}

void OLED_GameMode() {
	set_aktiv_meny(STATE_GAMEMODE, gamemode_line, ANTALL(gamemode_line), 0);
	OLED_POS(0, 8); OLED_print("Game mode");
	OLED_POS(3, 8); OLED_print("Single player");
	OLED_POS(7, 8); OLED_print("Back to menu");
	OLED_Pointer();
}

void OLED_Tutorial() {
	set_aktiv_meny(STATE_TUTORIAL, tutorial_line, ANTALL(tutorial_line), 0);
	OLED_POS(0, 8); OLED_print("Tutorial");
	OLED_POS(3, 8); OLED_print("Start tutorial");
	OLED_POS(7, 8); OLED_print("Back to menu");
	OLED_Pointer();
}

void OLED_Calibrate() {
	set_aktiv_meny(STATE_CALIBRATE, calibrate_line, ANTALL(calibrate_line), 0);
	OLED_POS(0, 8); OLED_print("Calibrate");
	OLED_POS(3, 8); OLED_print("Calibrate now");
	OLED_POS(7, 8); OLED_print("Back to menu");
	OLED_Pointer();
}

//Menu navigation 
void OLED_Menu_Select(Direction_t direction) {         // virker på alle sider
	if (direction != old_direction) {
		if (direction == DOWN && indeks < aktiv_lineSize - 1) {
			indeks++;
			OLED_Pointer();
		} else if (direction == UP && indeks > 0) {
			indeks--;
			OLED_Pointer();
		}
	}
	old_direction = direction;
}

void OLED_Menu_Enter() {
	if (JoyBtnPressed == 0) {
		return;
	}
	JoyBtnPressed = 0;

	if (menu_state == STATE_HOME) {                    // hovedmeny: åpne valget
		home_indeks = indeks;
		if      (indeks == 0) OLED_Play();
		else if (indeks == 1) OLED_GameMode();
		else if (indeks == 2) OLED_Tutorial();
		else if (indeks == 3) OLED_Calibrate();
		return;
	}

	if (indeks == aktiv_lineSize - 1) {                // "Back to menu" er alltid siste valg
		OLED_Menu_Home();
		return;
	}

	// Første valg på undersiden – handlinger legges inn her
	if (menu_state == STATE_PLAY) {
		// start spillet
	} else if (menu_state == STATE_GAMEMODE) {
		// velg single player
	} else if (menu_state == STATE_TUTORIAL) {
		// start tutorial
	} else if (menu_state == STATE_CALIBRATE) {
		// kjør kalibrering av joystick
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