/*
 * ADC.c
 *
 * Created: 22.09.2026 15:26:59
 *  Author: andrksel
 */ 


#include "ADC.h"

#define ADC_BASE ((volatile uint8_t *)0x1000)

/* MAX156 configuration bits */
#define ADC_ALL  (1u << 7)

/* BUSY connected to PE0 */
#define ADC_BUSY  PE0

void Init_ADC(void)
{
	// PD5 / OC1A as output - connected to CLK (pin9) MAX156
	DDRD |= (1 << PD5);

	// Toggle OC1A on compare match
	// Timer1 CTC mode
	TCCR1A = (1 << COM1A0);
	TCCR1B = (1 << WGM12) | (1 << CS10);

	// TOP / compare value
	OCR1A = 1;
		
	// BUSY as input
	DDRE &= ~(1 << ADC_BUSY);
}

uint8_t ADC_read_channel(uint8_t channel)
{
    uint8_t config;
	uint8_t value;

    channel &= 0x03;       // MAX156: channel 0-3

    config = ADC_ALL | channel;

    *ADC_BASE = config;
	
    /*
     * BUSY = 0 while converting.
     */
	
	// Vent først på at BUSY faktisk går LOW
	while (PINE & (1 << ADC_BUSY))
	{
	}
	
	while (!(PINE & (1 << ADC_BUSY))) //Vent mens BUSY e low. 
	{
		//printf("or if stuck here:\n");
	}
    /*
     * Memory read:
     * ATmega automatically generates /CS + /RD.
     */
	
	value = *ADC_BASE;
    return value;
}

//uint8_t mapValue(uint8_t value, uint8_t fromMinValue, uint8_t fromMaxValue, uint8_t toMinValue, uint8_t toMaxValue){
	//
	//double normalizedValue = (value - fromMinValue)/(fromMaxValue - fromMinValue);
	//
	//uint8_t mappedValue = ((uint8_t)normalizedValue * (toMaxValue - toMinValue) + toMinValue);
	//
	//return mappedValue;
//}

int16_t mapValue(int16_t value, int16_t fromMinValue, int16_t fromMaxValue, int16_t toMinValue, int16_t toMaxValue)
{
	double normalizedValue = (double)(value - fromMinValue) / (fromMaxValue - fromMinValue);

	int16_t mappedValue = (int16_t)(normalizedValue * (toMaxValue - toMinValue) + toMinValue);

	if (mappedValue > 100){
		mappedValue = 100;
	}
	if(mappedValue < -100){
		mappedValue = -100;
	}
	return mappedValue;
}


void ADC_Print(int16_t value0, int16_t value1, int16_t x,int16_t y )
{
	printf(
	"ValueCh0: %d, ValueCh1: %d y-axis: %d x-axis: %d\n",
	value0,value1,y,x	
	);
}
