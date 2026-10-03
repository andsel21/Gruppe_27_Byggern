
/*
 * SPI.h
 *
 * Created: 03.10.2026 16:29:20
 *  Author: andrksel
 */ 
#pragma once

#include <avr/io.h>

typedef enum{
	SPI_ID_OLED,
	SPI_ID_CAN,
	SS3
	}SPI_CS_t;

void Init_SPI(void);
uint8_t SPI_Transfer(uint8_t byte, SPI_CS_t CS);
