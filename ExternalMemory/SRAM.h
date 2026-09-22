/*
 * SRAM.h
 *
 * Created: 15.09.2026 15:06:27
 *  Author: andrksel
 */ 

#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>

void SRAM_test(void);
void InitSRAM(void);
uint8_t ADC_read_channel(uint8_t channel);

