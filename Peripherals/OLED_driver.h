#pragma once

#include <avr/io.h>
#include <avr/delay.h>

#include "Joystick.h"
#include "../Communication/SPI_driver.h"

typedef enum {
    STATE_HOME,
    STATE_PLAY,
    STATE_GAMEMODE,
    STATE_TUTORIAL,
    STATE_CALIBRATE,
    STATE_Bilde
} MenuState_t;

extern MenuState_t menu_state;

void Init_OLED(void);
void OLED_clear(void);
void OLED_goto_line(uint8_t line);
void OLED_goto_column(uint8_t column);
void OLED_POS(uint8_t page, uint8_t column);
void OLED_write_data(uint8_t data);
void OLED_print_arrow(uint8_t page, uint8_t column);
void OLED_write_command(uint8_t command);
void OLED_clear_line(uint8_t page);
void OLED_home(void);
void OLED_print_char(char c);
void OLED_print(const char *str);

// Shared menu API. Open/Move run in the main loop; Enter queues a press.
// Move must be called regularly to process queued button presses.
void OLED_Menu_Open(MenuState_t state);
void OLED_Menu_Move(Direction_t direction);
void OLED_Menu_Enter(void);

// Existing names remain available for main.c and Joystick.c.
void OLED_Menu_Home(void);
void OLED_Menu_Select(void);
void OLED_ReturnToHomeMenu(void);

void OLED_Play(void);
void OLED_GameMode(void);
void OLED_Tutorial(void);
void OLED_Calibrate(void);

void OLED_Menu_Pointer(uint8_t pointer);
void OLED_Play_Pointer(uint8_t pointer);
void OLED_GameMode_Pointer(uint8_t pointer);
void OLED_Tutorial_Pointer(uint8_t pointer);
void OLED_Calibrate_Pointer(uint8_t pointer);       

void OLED_Menu_Home_Select(Direction_t direction);
void OLED_Play_Select(Direction_t direction);
void OLED_Tutorial_Select(Direction_t direction);
void OLED_Calibrate_Select(Direction_t direction);
void OLED_GameMode_Select(Direction_t direction);
