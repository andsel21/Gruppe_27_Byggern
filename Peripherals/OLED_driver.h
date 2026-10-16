

#pragma once

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

#include "Joystick.h"
#include "../Communication/SPI_driver.h"

typedef enum {
    STATE_HOME,
    STATE_PLAY,
    STATE_GAMEMODE,
    STATE_TUTORIAL,
    STATE_CALIBRATE
} MenuState_t;

/* Driver */
void Init_OLED();
void OLED_reset();
void OLED_goto_line(uint8_t line);
void OLED_goto_column(uint8_t column);
void OLED_POS(uint8_t page, uint8_t column);
void OLED_clear_line(uint8_t page);
void OLED_home();
void OLED_write_data(uint8_t data);
void OLED_write_command(uint8_t command);
void OLED_print_char(char c);
void OLED_print(const char* str);
void OLED_print_arrow(uint8_t page, uint8_t column);

/* Meny */
void OLED_Menu_Home();
void OLED_Play();
void OLED_GameMode();
void OLED_Tutorial();
void OLED_Calibrate();
void OLED_Menu_Select(Direction_t direction);
void OLED_Menu_Enter();