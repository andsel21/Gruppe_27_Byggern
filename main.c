#ifndef F_CPU
#define F_CPU 4915200UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include "Communication/UART_driver.h"
#include "Utils/BitHandling.h"
#include "TEST_Functions/TEST_Functions.h"
#include "ExternalMemory/SRAM.h"
#include "Peripherals/ADC.h"
#include "Peripherals/Joystick.h"


//Legger til tekst for git check

/* =========================================================
   MAIN
   ========================================================= */

//typedef struct joystick_Dir {
	//uint8_t NEUTRAL,
	//uint8_t LEFT,
	//uint8_t RIGHT,
	//uint8_t UP,
	//uint8_t DOWN,
//}joystick_Dir_t;
//



int main(void)
{

	
	struct MemoryReadWrite SRAMaddress = {
		.package = 0x0F,
		.package_size = 2,
		.PTR = (volatile uint8_t*)0x1800
	};
	
	struct joystick_io js;
	
	joystickINIT(js);

	
	uint8_t adc_value;
			
	uint8_t ArrayToHoldTestData[SRAMaddress.package_size];
	
    UART_init();

    sei(); // Enable global interrupts
	
	InitSRAM();
	Init_ADC();
	
	//SRAM_test();

	uint8_t analogValue;

	while (1)
    {
		SRAMaddress.PTR = (volatile uint8_t*)0x1800;
		//adc_value = ADC_read_channel(0);
		//printf("ADC channel:0 value: %d\n",adc_value);
		//ADC_Print(0,ADC_read_channel(0));
		
		//joystick_Dir_t myDir;
		
		uint8_t var0 = ADC_read_channel(0);
		uint8_t var1 = ADC_read_channel(1);
		int16_t Raw_x = ADC_read_channel(2);
		int16_t Raw_y = ADC_read_channel(3);
		
		int16_t scaledX = mapValue(Raw_x, 64, 255, -100, 100);
		int16_t scaledY = mapValue(Raw_y, 77, 242, -100, 100);
		
		int16_t platX  = var0;
		int16_t platY = var1;
		
		
				
				////kvadrant 1
				//if(scaledX > 0 && scaledY > 0){
					//if(scaledX > scaledY){
						//myDir = RIGHT;
						//}else{
						//myDir = UP;
					//}
				//}
//
				////kvadrant 2
				//if(scaledX < 0 && scaledY > 0){
					//if(abs(scaledX) > abs(scaledY)){
						//myDir = LEFT;
						//}else{
						//myDir = UP;
					//}
				//}
//
				////kvadrant 3
				//if(scaledY<0 && scaledX > 0){
					//if(abs(scaledX) > abs(scaledY)){
						//myDir = DOWN;
						//}else{
						//myDir = RIGHT;
					//}
				//}
//
				////kvadrant 4
				//if(scaledX < 0 && scaledY < 0){
					//if(scaledX < scaledY){
						//myDir = LEFT;
						//}else{
						//myDir = DOWN;
					//}
				//}
		
		ADC_Print(var0, var1,scaledX,scaledY);
		
	
		//printf("%s",myDir);
		
		
//
		//analogValue = ADC_read_channel(0);
		//ADC_Print(0, analogValue);
		//_delay_ms(125);
	    //analogValue = ADC_read_channel(1);
		//ADC_Print(1, analogValue);
		//_delay_ms(125);
		//analogValue = ADC_read_channel(2);
		//ADC_Print(2, analogValue);
		//_delay_ms(125);
		//analogValue = ADC_read_channel(3);
		//ADC_Print(3, analogValue);
		//printf("\n");
		//_delay_ms(125);


        /*
         * Print result
         */
        //ADC_Print(2, analogValue);


        _delay_ms(100);
		
		sqaureWaveFuncPB0();

		////SRAMaddress.PTR = 0x1800;
		//WriteSRAM(SRAMaddress);
		////SRAMaddress.PTR = 0x1800;
		//ReadSRAM(SRAMaddress, ArrayToHoldTestData);
		//
		//SRAM_test();
	
////
		//uint8_t i = 0;
		//uint8_t randomtall = 0xFF;
		//for (i=0; i < 4; i++)
		//{
			//ext_ram[i] = randomtall;
			//uint8_t readValue = ext_ram[i];
			//
			//printf("readValue: %d\n",readValue);
		//}
		//_delay_ms(250);
		
		
		
	
		
        //UART_sendString("Hello world!\r\n"); <---> old code
		//printf("Hello World!\n");
		
		
		//adc_value = ADC_read_channel(0);
		//printf("Her kommer value: %d\n",adc_value);	
		

		//ext_ram = 0x1800;
		//for (i =0; i < 2; i++)
		//{
			//ext_ram[i] = randomtall;
			//uint8_t readValue = ext_ram[i];
			//
			//printf("readValue: %d\n",readValue);
		//}
				

    }
}
