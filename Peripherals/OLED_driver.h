#pragma once

#include <avr/io.h>
#include <avr/delay.h>

#include "Joystick.h"

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
void OLED_Menu_POINTER(uint8_t pointer);
void OLED_Menu_Select(Direction_t direction);
