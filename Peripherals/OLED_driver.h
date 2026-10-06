#pragma once

#include <avr/io.h>
#include <avr/delay.h>

void Init_OLED(void);
void reset();
void OLED_POS(uint8_t page, uint8_t column);
void OLED_write_data(uint8_t data);
void OLED_print_arrow(uint8_t page, uint8_t column);