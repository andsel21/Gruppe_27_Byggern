#include "PinStructure.h"



PinStruct_t PINS = {
    .A_Config = &DDRA,
    .B_Config = &DDRB,
    .C_Config = &DDRC,
    .D_Config = &DDRD,

    .A_Set_H_L = &PORTA,
    .B_Set_H_L = &PORTB,
    .C_Set_H_L = &PORTC,
    .D_Set_H_L = &PORTD,

	.A_Status = &PINA,
	.B_Status = &PINB,
	.C_Status = &PINC,
	.D_Status = &PIND,
	.E_Status = &PINE
};
