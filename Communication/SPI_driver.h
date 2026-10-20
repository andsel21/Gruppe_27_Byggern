
/*
 * SPI.h
 *
 * Created: 03.10.2026 16:29:20
 *  Author: andrksel
 */ 
#pragma once

#include <avr/io.h>
#include <avr/delay.h>

uint8_t writeBYTE;

typedef struct __attribute__((packed)) {
    union {
        uint8_t right;
        struct {
            uint8_t R1:1;
            uint8_t R2:1;
            uint8_t R3:1;
            uint8_t R4:1;
            uint8_t R5:1;
            uint8_t R6:1;
        };
    };
    union {
        uint8_t left;
        struct {
            uint8_t L1:1;
            uint8_t L2:1;
            uint8_t L3:1;
            uint8_t L4:1;
            uint8_t L5:1;
            uint8_t L6:1;
            uint8_t L7:1;
        };
    };
    union {
        uint8_t nav;
        struct {
            uint8_t NB:1;
            uint8_t NR:1;
            uint8_t ND:1;
            uint8_t NL:1;
            uint8_t NU:1;
        };
    };
} Buttons;

typedef enum{
	SPI_ID_OLED,
	SPI_ID_CONTROLLER,
	SPI_ID_CAN,
	SS3
	}SPI_CS_t;

void Init_SPI(void);
uint8_t SPI_Transfer(uint8_t byte, SPI_CS_t CS, uint8_t D_C);


void Board_LEDS(uint8_t LED, uint8_t State);
void Board_PWM_LEDS(uint8_t LED, uint8_t PWM);
void Board_Buttons();