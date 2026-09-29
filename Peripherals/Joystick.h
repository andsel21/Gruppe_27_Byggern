
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

struct joystick_io{
	uint8_t x_axis;
	uint8_t y_axis;
	bool btn;
};

void joystickINIT(struct joystick_io js);
void ReadJoystickPos(struct joystick_io io);
void btnRead(joystick_io.btn);
