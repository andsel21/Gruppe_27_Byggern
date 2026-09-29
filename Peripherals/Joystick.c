
/*
 * Joystick.c
 *
 * Created: 29.09.2026 12:52:55
 *  Author: andrksel
 */ 

#include "Joystick.h"


void joystickINIT(struct joystick_io js){
	js.btn = false;
	js.x_axis = 0;
	js.y_axis = 0;
}

//void ReadJoystickPos(int16_t scaledX,int16_t scaledY, enum joystick_Dir dir){
//
//}

void btnRead(struct joystick_io io){
	if(io.btn == 1){
		printf("BTN pressed");
		}else{
		printf("Btn NOT Pressed");
	}
	
}


void Read(){
			uint8_t var0 = ADC_read_channel(0);
			uint8_t var1 = ADC_read_channel(1);
			int16_t Raw_x = ADC_read_channel(2);
			int16_t Raw_y = ADC_read_channel(3);
			
			int16_t scaledX = mapValue(Raw_x, 64, 255, -100, 100);
			int16_t scaledY = mapValue(Raw_y, 77, 242, -100, 100);
			
			int16_t platX  = var0;
			int16_t platY = var1;
	
}
