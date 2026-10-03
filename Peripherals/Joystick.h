
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
	int16_t scaled_x_axis;
	int16_t scaled_y_axis;
	bool btn;
}joystick_io_t;


void joystickINIT(joystick_io_t *js);

joystick_io_t ReadAndScale();
void JoyStickPos_Print(joystick_io_t *js);
Direction_t JoyDirection(joystick_io_t *js);
