
/*
 * Joystick.h
 *
 * Created: 29.09.2026 12:53:08
 *  Author: andrksel
 */ 

#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>
#include <stdbool.h>
#include <avr/interrupt.h>

volatile uint8_t buttonPressed;

typedef enum {
	NEUTRAL,
	LEFT,
	RIGHT,
	DOWN,
	UP
}Direction_t;


typedef struct {
	uint8_t RAW_x_axis;
	uint8_t RAW_y_axis;
	uint8_t RAW_x_joypad;
	uint8_t RAW_y_joypad;
	int16_t scaled_x_axis;
	int16_t scaled_y_axis;
	int16_t scaled_x_joypad;
	int16_t scaled_y_joypad;
	bool btn;
}joystick_io_t;


void joystickINIT(joystick_io_t *js);

void ReadAndScale(joystick_io_t *js);
void JoyStickPos_Print(joystick_io_t *js);
Direction_t JoyDirection(joystick_io_t *js);
