
/*
 * Joystick.c
 *
 * Created: 29.09.2026 12:52:55
 *  Author: andrksel
 */ 

#include "Joystick.h"
#include "ADC.h"
#include "../Utils/ScaleMapping.h"



void joystickINIT(joystick_io_t *js){
	js->btn = false;
	js->RAW_x_axis = 0;
	js->RAW_y_axis = 0;
	js->scaled_x_axis = 0;
	js->scaled_y_axis = 0;
}

//void ReadJoystickPos(int16_t scaledX,int16_t scaledY, enum joystick_Dir dir){
//
//}

// void btnRead(struct joystick_io io){
// 	if(io.btn == 1){
// 		printf("BTN pressed");
// 		}else{
// 		printf("Btn NOT Pressed");
// 	}
	
// }


joystick_io_t ReadAndScale(){
			uint8_t PlatRawX = ADC_read_channel(0);
			uint8_t PlatRawY = ADC_read_channel(1);
			int16_t JoyRaw_x = ADC_read_channel(2);
			int16_t JoyRaw_y = ADC_read_channel(3);
			
			int16_t scaledX = mapValue(JoyRaw_x, 64, 255, -100, 100);
			int16_t scaledY = mapValue(JoyRaw_y, 77, 242, -100, 100);
			

			//NEEDS TO BE CONFIGURED FOR PLATFORMS RAW VALUES
			int16_t scaledPlatX = mapValue(PlatRawX, 64, 255, -100, 100);
			int16_t scaledPlatY = mapValue(PlatRawY, 77, 242, -100, 100);
	
}


void JoyStickPos_Print(joystick_io_t *js)
{
	printf(
	"ValueCh0: %d, ValueCh1: %d y-axis: %d x-axis: %d\n",
	js->RAW_x_axis,
	js->RAW_y_axis,
	js->scaled_x_axis,
	js->scaled_y_axis
	);
}


Direction_t JoyDirection(joystick_io_t *js){
					//kvadrant 1
				if(js->scaled_x_axis > 0 && js->scaled_y_axis > 0){
					if(js->scaled_x_axis > js->scaled_y_axis){
					return RIGHT;
						}else{
						return UP;
					}
				}

				//kvadrant 2
				if(js->scaled_x_axis < 0 && js->scaled_y_axis > 0){
					if(abs(js->scaled_x_axis) > abs(js->scaled_y_axis)){
						return LEFT;
						}else{
						return UP;
					}
				}

				//kvadrant 3
				if(js->scaled_y_axis<0 && js->scaled_x_axis > 0){
					if(abs(js->scaled_x_axis) > abs(js->scaled_y_axis)){
						return DOWN;
						}else{
						return RIGHT;
					}
				}

				//kvadrant 4
				if(js->scaled_x_axis < 0 && js->scaled_y_axis < 0){
					if(js->scaled_x_axis < js->scaled_y_axis){
						return LEFT;
						}else{
						return DOWN;
					}
				}

}