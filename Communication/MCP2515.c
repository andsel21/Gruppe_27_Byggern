
#include "MCP2515.h"
#include "../Utils/PIN_Alias.h"
#define mcp2515_resetregister 0xC0 //1100 0000
#define mcp2515_readregister 0x03 //0000 0011
#define mcp2515_canstat 0x0E  //

uint8_t mcp2515_read(uint8_t adr){

    uint8_t value = 0;

    SPI_Transfer(mcp2515_readregister, SPI_ID_CAN, 0);
    SPI_Transfer(adr, SPI_ID_CAN, 0);
    value = SPI_Transfer(0x00, SPI_ID_CAN, 0);
     
    PORTB |= (1 << PB1_CAN_CS); //CS high

    return value;

}

// void mcp2515_write(uint8_t adr, uint8_t data);
//     SPI_Transfer(mcp2515_read, SPI_ID_CAN, 0);


// void mcp2515_request(uint8_t adr);
//     SPI_Transfer(mcp2515_read, SPI_ID_CAN, 0);


// uint8_t mcp2515_read_status();

// void mcp2515_bit();

void mcp2515_reset() {

    SPI_Transfer(mcp2515_resetregister, SPI_ID_CAN, 0);
     
    PORTB |= (1 << PB1_CAN_CS);

    _delay_ms(10);
}


void mcp2515_test(){
    mcp2515_reset();

    uint8_t canstat = mcp2515_read(mcp2515_canstat);

    printf("canstat %d \n", canstat);

    if (canstat == 0x80) {
        printf("Svarer \n");
    } else {
        printf("Feil \n");
    }
}