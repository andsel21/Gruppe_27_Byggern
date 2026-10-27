
#include "OLED_driver.h"
#include "../Communication/SPI_driver.h"
#include "../fonts.h"

extern SPI_CS_t ChipSelect;

MenuState_t menu_state = STATE_HOME;
static uint8_t indeks = 0;
static Direction_t old_direction = NEUTRAL;
extern volatile uint8_t JoyBtnPressed;
extern uint8_t EnableSW;

void Init_OLED(void){
	//PB2 = DISP D/C#
	DDRB |= (1 << PB2); //Setter PB2 som output for OLED D/C#
	DDRB |= (1 << PB3); //Setter PB3 som output for OLED CS
	PORTB &= ~(1 <<PB2); //Setter D/C# til 0 for command
	PORTB &= ~(1 <<PB3); //Setter CS til 0 for command
	OLED_write_command(0xAE); // Display OFF normal Mode
	_delay_ms(100);

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

void OLED_print(const char* str){
	while(*str){
		OLED_print_char(*str);
		str++;
	}
}

// Teller som tekst, funksjonkall eller meny endring
typedef enum {
    TEXT,
    FUNCTION,
    MENU
} MenuLineType_t;

// Hva som "står" i en linje/Page
typedef struct {
    const char *text;
    uint8_t page;
    uint8_t column;

    MenuLineType_t type;

    MenuState_t TargetMenu;
    void (*function)(void);
} MenuLines_t;

// Hva en meny inneholder, meny -> linje -> type
typedef struct {
    const MenuLines_t *Lines;
    uint8_t count;
} Menu_t;

void LED_ON(){
    EnableSW = 1;
}
void LED_OFF(){
    EnableSW = 0;
}

// {TEXT, Line, Collum, Type, Targetmenu, functionCall}
const static const MenuLines_t Menu_Home[] = {
    {"Ping-Pong Game", 0, 8,  TEXT,     0,               NULL},
    {"Play",           1, 8,  MENU,     STATE_PLAY,      NULL},
    {"Game mode",      3, 8,  MENU,     STATE_GAMEMODE,  NULL},
    {"Tutorial",       5, 8,  MENU,     STATE_TUTORIAL,  NULL},
    {"Calibrate",      7, 8,  MENU,     STATE_CALIBRATE, NULL}
};

const static const MenuLines_t Menu_Play[] = {
    {"Playing",        0, 20, TEXT,     0,               NULL},
    {"Return",         7, 20, MENU,     STATE_HOME,      NULL}
};

const static const MenuLines_t Menu_Gamemode[] = {
    {"Game mode",      0, 20, TEXT, 0,                   NULL},
    {"Return",         7, 20, MENU,     STATE_HOME,      NULL}
};

const static const MenuLines_t Menu_Tutorial[] = {
    {"Tutorial",       0, 20, TEXT,     0,               NULL},
    {"bob",            3, 20, MENU,     STATE_PLAY,      NULL},
    {"Return",         7, 20, MENU,     STATE_HOME,      NULL}
};

const static const MenuLines_t Menu_Calibrate[] = {
    {"Calibrate",      0, 20, TEXT, 0,                   NULL},
    {"Led Blink On",   1, 20, FUNCTION, 0,               LED_ON},
    {"Led Blink Off",  2, 20, FUNCTION, 0,               LED_OFF},
    {"X:",             3, 20, TEXT,     0,               NULL},
    {"Y:",             4, 20, TEXT,     0,               NULL},
    {"Return",         7, 20, MENU,     STATE_HOME,      NULL}
};

// Hold menu as array with ammount of lines.
const static const Menu_t menus[] = {
    [STATE_HOME]      = {Menu_Home,      sizeof(Menu_Home) / sizeof(Menu_Home[0])},
    [STATE_PLAY]      = {Menu_Play,      sizeof(Menu_Play) / sizeof(Menu_Play[0])},
    [STATE_GAMEMODE]  = {Menu_Gamemode,  sizeof(Menu_Gamemode) / sizeof(Menu_Gamemode[0])},
    [STATE_TUTORIAL]  = {Menu_Tutorial,  sizeof(Menu_Tutorial) / sizeof(Menu_Tutorial[0])},
    [STATE_CALIBRATE] = {Menu_Calibrate, sizeof(Menu_Calibrate) / sizeof(Menu_Calibrate[0])}
};

/* =========================================================
   TO DO 
   ========================================================= */
//
// / Peker på meny Home
// / const Menu_t *menu = &menus[STATE_HOME];
// / Peker på velgbare linje i en menu (Ikke i global )
// / const MenuLines_t *item = &menu->Lines[0];



// Peker på meny, Peker på linjer, hent linje(i) gå til posisjon og skriv tekst
void OLED_DrawMenu(void) {
    const Menu_t *menu = &menus[menu_state];
    OLED_reset();

    for (uint8_t i = 0; i < menu->count; i++) {
        const MenuLines_t *Line = &menu->Lines[i];
        OLED_POS(Line->page, Line->column);
        OLED_print(Line->text);
    }
    OLED_Pointer(indeks);
}

// Chooses where to point >
void OLED_Pointer(uint8_t indeks) {

    const Menu_t *menu = &menus[menu_state];
    uint8_t selectableIndex = 0;

    // Loop Menu 0-7 pages max
    for (uint8_t i = 0; i < menu->count; i++) {
        const MenuLines_t *Line = &menu->Lines[i];

        // Text-only Lines are not selected
        if (Line->type == TEXT) {
            continue;
        }

        // Print > or delete previous. Uppdates all pages
        OLED_POS(Line->page, 0);
        if (selectableIndex == indeks) {
            OLED_print_char('>');
        }
        else {
            OLED_print_char(' ');
        }

        selectableIndex++;
    }
}

void OLED_Menu_Action(void) {

    if (JoyBtnPressed == 0){
        return;
    } 
    JoyBtnPressed = 0;

    const Menu_t *menu = &menus[menu_state];
    uint8_t selectableIndex = 0;

    // Runs for all Pages or desired line is found
    for (uint8_t i = 0; i < menu->count; i++) {
        const MenuLines_t *Line = &menu->Lines[i];

        // Go to next index if it is text
        if (Line->type == TEXT) {
            continue;
        }
        // If current index match selectable
        if (selectableIndex == indeks) {

            // Go to target menu of line
            if (Line->type == MENU) {
                menu_state = Line->TargetMenu;
                indeks = 0;
                OLED_DrawMenu();
            }
            // Go to function
            else if (Line->type == FUNCTION) {

                if (Line->function != NULL) {
                    Line->function();
                }
            }
            return;
        }
        selectableIndex++;
    }
}

void OLED_Select(Direction_t direction) {

    const Menu_t *menu = &menus[menu_state];

    uint8_t selectableCount = 0;

    for (uint8_t i = 0; i < menu->count; i++) {

        if (menu->Lines[i].type != TEXT) {
            selectableCount++;
        }
    }
    if (direction != old_direction) {
        if (direction == DOWN && indeks + 1 < selectableCount) {
            indeks++;
            OLED_Pointer(indeks);
        }
        else if (direction == UP && indeks > 0) {
            indeks--;
            OLED_Pointer(indeks);
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