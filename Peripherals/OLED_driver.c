
#include "OLED_driver.h"
#include "../fonts.h"

MenuState_t menu_state = STATE_HOME;

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

void OLED_clear(void){ // Clear display RAM; this is not a hardware reset.
	for (uint8_t page = 0; page < 8; page++) {
		OLED_clear_line(page);
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

void OLED_home(void){
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
        OLED_write_data(pgm_read_byte(&font8[c - 32][i])); //c -32 er for å match vår indeks i font8
    }
}

void OLED_print(const char *str){
	while(*str){
		OLED_print_char(*str);
		str++;
	}
}



/* Add menu entries here. An entry opens its target screen.
 * Labels use RAM strings, like OLED_print(). Pages are 0..7.
 */
typedef struct {
    const char *label;
    uint8_t page;
    MenuState_t target;
} OLED_MenuItem_t;

typedef struct {
    const char *title; // Optional title on page 0, column 8.
    const OLED_MenuItem_t *items;
    uint8_t count;
    uint8_t text_column;
} OLED_Menu_t;

#define ITEM_COUNT(items) (sizeof(items) / sizeof((items)[0]))

static const OLED_MenuItem_t home_items[] = {
    {"Play",      1, STATE_PLAY},
    {"Game mode", 3, STATE_GAMEMODE},
    {"Tutorial",  5, STATE_TUTORIAL},
    {"Calibrate", 6, STATE_CALIBRATE},
    {"Bilde", 7, STATE_Bilde}
};
static const OLED_MenuItem_t play_items[] = {
    {"Playing", 3, STATE_PLAY},
    {"Return",  7, STATE_HOME}
};
static const OLED_MenuItem_t gamemode_items[] = {
    {"Game mode", 3, STATE_GAMEMODE},
    {"Return",    7, STATE_HOME}
};
static const OLED_MenuItem_t tutorial_items[] = {
    {"Tutorial", 0, STATE_TUTORIAL},
    {"bob",      3, STATE_PLAY},
    {"Return",   7, STATE_HOME}
};
static const OLED_MenuItem_t calibrate_items[] = {
    {"Calibrate", 3, STATE_CALIBRATE},
    {"Return",    7, STATE_HOME}
};

static const OLED_MenuItem_t bilde_items[] = {
    {"Turn On LED", 0, STATE_Bilde},
    {"bilde", 2, STATE_Bilde},
    {"bilde", 3, STATE_Bilde},
    {"Return",    7, STATE_HOME}
};

// New screens need a state in OLED_driver.h and a descriptor here.
// Their main-loop caller must also call OLED_Menu_Move(): main.c currently
// dispatches only the five existing states through the compatibility wrappers.
static const OLED_Menu_t menus[] = {
    [STATE_HOME]      = {"Ping-Pong Game", home_items, ITEM_COUNT(home_items), 8},
    [STATE_PLAY]      = {NULL, play_items, ITEM_COUNT(play_items), 20},
    [STATE_GAMEMODE]  = {NULL, gamemode_items, ITEM_COUNT(gamemode_items), 20},
    [STATE_TUTORIAL]  = {NULL, tutorial_items, ITEM_COUNT(tutorial_items), 20},
    [STATE_CALIBRATE] = {NULL, calibrate_items, ITEM_COUNT(calibrate_items), 20},
    [STATE_Bilde] = {NULL, bilde_items, ITEM_COUNT(bilde_items), 20}
};

static uint8_t selected = 0;
static Direction_t old_direction = NEUTRAL;
static volatile uint8_t menu_ready = 0; // Also read by the button ISR.
extern volatile uint8_t JoyBtnPressed;

static void menu_draw_pointer(MenuState_t state, uint8_t index)
{
    if ((unsigned int)state >= ITEM_COUNT(menus)) return;
    const OLED_Menu_t *menu = &menus[state];
    for (uint8_t i = 0; i < menu->count; i++) {
        OLED_POS(menu->items[i].page, 0);
        OLED_print_char(i == index ? '>' : ' ');
    }
}

void OLED_Menu_Open(MenuState_t state)
{
    const OLED_Menu_t *menu = &menus[state];

    menu_state = state;
    selected = 0;
    OLED_clear();
    if (menu->title != NULL) {
        OLED_POS(0, 8);
        OLED_print(menu->title);
    }
    for (uint8_t i = 0; i < menu->count; i++) {
        OLED_POS(menu->items[i].page, menu->text_column);
        OLED_print(menu->items[i].label);
    }
    menu_draw_pointer(state, selected);
    menu_ready = 1;
}

// Point to selected menu, then selected item,
static void menu_activate(void)
{
    const OLED_Menu_t *menu = &menus[menu_state];
    const OLED_MenuItem_t *item = &menu->items[selected];
    OLED_Menu_Open(item->target);
}

void OLED_Menu_Move(Direction_t direction)
{
    if (!menu_ready) return;

    // Joystick.c sets this flag and calls OLED_Menu_Enter() in INT0.
    // Consume it here so screen drawing and SPI run only in the main loop.
    uint8_t pressed;
    pressed = JoyBtnPressed;
    JoyBtnPressed = 0;

    if (pressed) {
        old_direction = direction;
        menu_activate();
        return; // A press only activates one item, even after changing screens.
    }

    if ((unsigned int)menu_state >= ITEM_COUNT(menus)) return;
    const OLED_Menu_t *menu = &menus[menu_state];
    uint8_t previous = selected;

    if (direction != old_direction) {
        if (direction == DOWN && selected + 1 < menu->count) {
            selected++;
        } else if (direction == UP && selected > 0) {
            selected--;
        }
    }
    old_direction = direction;
    if (selected != previous) menu_draw_pointer(menu_state, selected);
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
