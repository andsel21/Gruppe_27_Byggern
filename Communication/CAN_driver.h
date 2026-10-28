#pragma once 
#include <stdio.h>
#include <avr/io.h>
#include "SPI_driver.h"

// uint8_t CAN_driver(uint8_t adr);
// void mcp2515_reset();
// void mcp2515_test();

void Init_CAN(uint8_t mode);
uint8_t CAN_Read(uint8_t adr);
void CAN_test(uint8_t mode);
void CAN_Write(uint8_t *data,uint8_t litteral, uint8_t size, uint8_t newMsg, uint8_t deselect);
void CAN_test(uint8_t mode);
void CAN_Reset();


// void CAN_Request_To_Send2(uint8_t buffer);
// uint8_t CAN_SendTest(void);