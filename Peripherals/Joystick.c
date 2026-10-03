
/*
 * Joystick.c
 *
 * Created: 29.09.2026 12:52:55
 *  Author: andrksel
 */ 

#include "Joystick.h"
#include "ADC.h"
#include "../Utils/ScaleMapping.h"

volatile uint8_t JoyBtnPressed;


void joystickINIT(joystick_io_t *js){
	js->btn = false;
	js->RAW_x_axis = 0;
	js->RAW_y_axis = 0;
	js->RAW_x_joypad = 0;
	js->RAW_y_joypad = 0;
	js->scaled_x_axis = 0;
	js->scaled_y_axis = 0;
	js->scaled_x_joypad = 0;
	js->scaled_y_joypad = 0;
		
	//Konfigureres til Rising Edge		
	MCUCR |= (1<<ISC00);
	MCUCR |= (1<<ISC01);
	
	// Button External interrupt Enable
	GICR |= (1<<INT0);
	//SREG BIT 7 SETTES TIL 1 FRA SEI() I MAIN.
	//GIFR |= (1<<INTF0);
}

//
 //void btnRead(struct joystick_io io){
	 //if(io.btn == 1){
		 //printf("BTN pressed");
		 //}else{
		 //printf("Btn NOT Pressed");
	 //}
//}


void ReadAndScale(joystick_io_t *js){
		
	js->RAW_x_joypad = ADC_read_channel(0);
	js->RAW_y_joypad = ADC_read_channel(1);
	js->RAW_x_axis = ADC_read_channel(2);
	js->RAW_y_axis = ADC_read_channel(3);

	js->scaled_x_axis = mapValue(js->RAW_x_axis, 64, 255, -100, 100);
	js->scaled_y_axis = mapValue(js->RAW_y_axis, 77, 242, -100, 100);

	//NEEDS TO BE CONFIGURED FOR PLATFORMS RAW VALUES
	js->scaled_x_joypad = mapValue(js->RAW_x_axis, 0, 255, -100, 100);
	js->scaled_y_joypad = mapValue(js->RAW_y_axis, 0, 255, -100, 100);
	
}


void JoyStickPos_Print(joystick_io_t *js)
{
	printf(
	"ValueCh0: %d, ValueCh1: %d y-axis: %d x-axis: %d\n",
	js->RAW_y_axis,
	js->RAW_x_axis,
	js->scaled_y_axis,
	js->scaled_x_axis
	);
	//Legg til Joystick-PAD??
}


Direction_t JoyDirection(joystick_io_t *js){
					
	   int16_t x = js->scaled_x_axis;
	   int16_t y = js->scaled_y_axis;

	   int16_t deadBandX = 80;
	   int16_t deadBandY = 80;

	   // Inside deadband -> NEUTRAL
	   if (abs(x) <= deadBandX && abs(y) <= deadBandY)
	   {
		   return NEUTRAL;
	   }

	   // X-axis has the largest movement
	   if (abs(x) > abs(y))
	   {
		   if (x > 0)
		   {
			   return RIGHT;
		   }
		   else
		   {
			   return LEFT;
		   }
	   }

	   // Y-axis has the largest movement
	   else
	   {
		   if (y > 0)
		   {
			   return UP;
		   }
		   else
		   {
			   return DOWN;
		   }
	   }
	   
	   	//NEUTRAL - 0
	   	//LEFT	  - 1
	   	//RIGHT   - 2
	   	//DOWN    - 3
	   	//UP      - 4
}

ISR(INT0_vect){
	//Flagg toggles automatisk
	//Button External interrupt Disable
	GICR&= ~(1<<INT0);
	JoyBtnPressed += 1;
	

	
	// Reset Timer
	TCNT3 = 0;
	//Enable Timer1 Compare Match A Interrupt - starter timer!
	ETIMSK |= (1 << OCIE3A); //Slår på
	//Prescalar 64: Start Timer (CS11 = 1m CS10 = 1)
	TCCR3B |= (1<<CS31) | (1 << CS30);
	
}