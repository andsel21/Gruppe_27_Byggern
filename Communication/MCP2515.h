#pragma once 
#include <stdio.h>
#include <avr/io.h>
#include "SPI_driver.h"

uint8_t mcp2515_read(uint8_t adr);
void mcp2515_reset();
void mcp2515_test();
