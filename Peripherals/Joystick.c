
/*
 * Joystick.c
 *
 * Created: 29.09.2026 12:52:55
 *  Author: andrksel
 */ 

#include "Joystick.h"


void joystickINIT(struct joystick_io js){
	js.btn = 0;
	js.x_axis = 0;
	js.y_axis = 0;
}

void ReadJoystickPos(struct joystick_io io){
		
}

void btnRead(joystick_io.btn){
	if(joystick_io.btn == 1){
		printf("BTN pressed");
	}else{
		printf("Btn NOT Pressed");
	}
	
}