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
    STATE_CALIBRATE
} MenuState_t;

static MenuState_t menu_state = STATE_HOME;

void Init_OLED(void);
void OLED_reset();
void OLED_goto_line(uint8_t line);
void OLED_goto_column(uint8_t column);
void OLED_POS(uint8_t page, uint8_t column);
void OLED_write_data(uint8_t data);
void OLED_print_arrow(uint8_t page, uint8_t column);
void OLED_write_command(uint8_t command);
void OLED_clear_line(uint8_t page);
void OLED_home();
void OLED_print_char(char c);
void OLED_print(char* str);
void OLED_Menu_Home();
void OLED_Menu_Pointer(uint8_t pointer);
void OLED_Menu_Select(Direction_t direction);
void OLED_Menu_Enter();
void OLED_Play();      
void OLED_GameMode();
void OLED_Tutorial();
void OLED_Calibrate();
void OLED_Play_Pointer(uint8_t pointer);
void OLED_GameMode_Pointer(uint8_t pointer);
void OLED_Tutorial_Pointer(uint8_t pointer);
void OLED_Calibrate_Pointer(uint8_t pointer);       
void ReturnToHomeMenu();
void OLED_Calibrate_Select(Direction_t direction);
void OLED_Play_Select(Direction_t direction);
void OLED_Tutorial_Select(Direction_t direction);
