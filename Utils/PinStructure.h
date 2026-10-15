#pragma once

#include <avr/io.h>

#define DDB0_LED_PULSE_CONF DDB0
#define PB0_LED_PULSE PB0
#define PD2_JoyBtn PD2
#define CS30_Prescaler CS30
#define CS31_Prescaler CS31
#define CS32_Prescaler CS32
#define OCIE3A_EnableInterrupt OCIE3A
#define INTF2_ExternalInterruptFlag2 INTF2
#define INTF1_ExternalInterruptFlag1 INTF1
#define INTF0_ExternalInterruptFlag0 INTF0
#define INT0_ExternalInterrupt0 INT0
#define INT1_ExternalInterrupt1 INT1
#define PB2_D_C PB2


typedef struct PINS{
    volatile uint8_t *A_Config;
    volatile uint8_t *B_Config;
    volatile uint8_t *C_Config;
    volatile uint8_t *D_Config;
    volatile uint8_t *E_Config;

    volatile uint8_t *A_Set_H_L;
    volatile uint8_t *B_Set_H_L;
    volatile uint8_t *C_Set_H_L;
    volatile uint8_t *D_Set_H_L;
    volatile uint8_t *E_Set_H_L;

    volatile uint8_t *A_Status;
    volatile uint8_t *B_Status;
    volatile uint8_t *C_Status;
    volatile uint8_t *D_Status;
    volatile uint8_t *E_Status;

}PinStruct_t;


extern PinStruct_t PINS;

