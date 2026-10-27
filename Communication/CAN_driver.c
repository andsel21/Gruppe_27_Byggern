
#include "CAN_driver.h"
#include "../Utils/PIN_Alias.h"
#define CAN_resetregister 0xC0 //1100 0000
#define CAN_readregister 0x03 //0000 0011
#define CAN_canstat 0x0E  //
#define CAN_writeregister 0x02
#define CAN_readStatusRegister 0xA0
#define CAN_bitModify 0x05
#define CNF1 0x2A
#define CNF2 0x29
#define CNF3 0x28

// uint8_t CNF1 = 0x2A;
// uint8_t CNF2 = 0x29;
// uint8_t CNF3 = 0x28;

uint8_t CNF_arr[3] = {0x2A,0x29,0x28};

uint8_t *dummyPTR = NULL;

uint8_t CAN_Read(uint8_t adr){
    uint8_t value = 0;

    SPI_Transfer(CAN_readregister, SPI_ID_CAN, 0);
    SPI_Transfer(adr, SPI_ID_CAN, 0);
    value = SPI_Transfer(0x00, SPI_ID_CAN, 0);
     
    PORTB |= (1 << PB1_CAN_CS); //CS high

    return value;
}

static void CAN_WriteMessage(uint8_t data){
    SPI_Transfer(data, SPI_ID_CAN, 0);
}

void CAN_Write(uint8_t *data,uint8_t litteral, uint8_t size, uint8_t newMsg, uint8_t deselect){
    if(newMsg){
    SPI_Transfer(CAN_writeregister, SPI_ID_CAN, 0);
    }
    
    // if(litteral != NULL){
    //     CAN_WriteMessage(litteral); //data
    //     if(deselect){
    //       PORTB |= (1<<PB1_CAN_CS);
    //     }
    // }else{
    //    for (uint8_t i = 0; i < size; i++) {        
    //         CAN_WriteMessage(*data);
    //         data += 1;
    //         if(deselect){
    //           PORTB |= (1<<PB1_CAN_CS);}
    //     }
    // }

     CAN_WriteMessage(litteral); //data
        if(deselect){
          PORTB |= (1<<PB1_CAN_CS);}
}

uint8_t CAN_ReadStatus(){
    /*
    Bit	Navn	        Når bit = 1	                    Når bit = 0
    0	RX0IF	        Melding mottatt i RXB0	        Ingen ulest melding i RXB0
    1	RX1IF	        Melding mottatt i RXB1	        Ingen ulest melding i RXB1
    2	TXB0.TXREQ	    TXB0 venter på sending	        Ingen sending venter
    3	TX0IF	        Sending fra TXB0 er fullført	Ingen fullføringsflagg satt
    4	TXB1.TXREQ	    TXB1 venter på sending	        Ingen sending venter
    5	TX1IF	        Sending fra TXB1 er fullført	Ingen fullføringsflagg satt
    6	TXB2.TXREQ	    TXB2 venter på sending	        Ingen sending venter
    7	TX2IF	        Sending fra TXB2 er fullført	Ingen fullføringsflagg satt
    */
    SPI_Transfer(CAN_readStatusRegister, SPI_ID_CAN, 0);
    uint8_t message = SPI_Transfer(0x0, SPI_ID_CAN,0);
    PORTB |= (1 << PB1_CAN_CS); //CS high
    return message;
}



void CAN_Request_To_Send(uint8_t RTSbufferSelect){ //if RTSbufferSelect = 0, command ignored
    uint8_t RTS = 0x80;
    if (RTSbufferSelect > 3)
    {RTSbufferSelect = 3;}
    RTS |= (1<< RTSbufferSelect);
    SPI_Transfer(RTS, SPI_ID_CAN, 0);
}

void CAN_BitModify(const uint8_t bits,const  uint8_t mask, uint8_t reg){
    SPI_Transfer(CAN_bitModify, SPI_ID_CAN, 0);
    SPI_Transfer(reg, SPI_ID_CAN,0);  // 
    SPI_Transfer(mask, SPI_ID_CAN,0); // Hvilke bit skal modifiseres
    SPI_Transfer(bits, SPI_ID_CAN,0); // Set bits: 1 == High, 0 == Low
    PORTB |= (1 << PB1_CAN_CS);
}

void CAN_Reset(){
    SPI_Transfer(CAN_resetregister, SPI_ID_CAN, 0);
    PORTB |= (1<<PB1_CAN_CS);
}

/*
    BitRate = Fosc/(2*(BRP +1)*Ntq)
    Fosc = 4 915 200 Hz
    BRP = Baud Rate Prescalar register value
    Ntq = Number of timer qunata per bit

    Sample Ponter = (1 +7 +8)/() * 100%

*/

void Init_CAN(uint8_t mode){ //1 == Testmode
// MCP2515 — 4.9152 MHz
// Bitrate: 102400 bit/s
// Sample point: 75%

    // SPI_Transfer(CAN_writeregister, SPI_ID_CAN, 0);
    // SPI_Transfer(CAN_resetregister, SPI_ID_CAN, 0);
    // PORTB |= (1<<PB1_CAN_CS);

    // SPI_Transfer(CAN_writeregister, SPI_ID_CAN, 0);
    // CAN_Write(dummyPTR,CNF1, 1); // BRP = 1, SJW = 1 TQ
    // CAN_Write(dummyPTR,0x01, 1); // data
    // PORTB |= (1<<PB1_CAN_CS);

    // SPI_Transfer(CAN_writeregister, SPI_ID_CAN, 0);
    // CAN_Write(dummyPTR,CNF2, 1); //PropSqg = 3, PhaseSeg1 = 5
    //  CAN_Write(dummyPTR,0xA2, 1); // data
    // PORTB |= (1<<PB1_CAN_CS);

    // SPI_Transfer(CAN_writeregister, SPI_ID_CAN, 0);
    // CAN_Write(dummyPTR,CNF3, 1); //PhaseSeg2 = 3
    //  CAN_Write(dummyPTR,0x02, 1); // data
    // PORTB |= (1<<PB1_CAN_CS);

    // SPI_Transfer(CAN_writeregister, SPI_ID_CAN, 0);
    // CAN_Write(dummyPTR,0x0, 1); // NormalMode
    // PORTB |= (1<<PB1_CAN_CS);


    CAN_Write(dummyPTR,0xC0, 1,0, 1); //RESET
    _delay_ms(10);

    CAN_Write(dummyPTR,CNF1, 1,1, 0); // BRP = 1, SJW = 1 TQ
    CAN_Write(dummyPTR,0x01, 1,0, 1); 
    CAN_Write(dummyPTR,CNF2, 1,1, 0); // PropSeg = 3, PhaseSeg1 = 5
    CAN_Write(dummyPTR,0xA2, 1,0, 1);
    CAN_Write(dummyPTR,CNF3, 1,1, 0); // PhaseSeg2 = 3
    CAN_Write(dummyPTR,0x02, 1,0, 1);
    
    CAN_Write(dummyPTR,0xF, 1,1, 0); //Normal Mode
    CAN_Write(dummyPTR,0x40, 1,0, 1); //0x0 = normal,  0x40 = loopback




}
/*
    AI KODE

// */
// void CAN_Request_To_Send2(uint8_t buffer)
// {
//     if (buffer > 2) {
//         return;
//     }

//     SPI_Transfer(0x80 | (1 << buffer), SPI_ID_CAN, 0);
//     PORTB |= (1 << PB1_CAN_CS);
// }

// uint8_t CAN_SendTest(void)
// {
//     // Krever Normal mode.
//     if ((CAN_Read(0x0E) & 0xE0) != 0x00) {
//         return 0;
//     }

//     // Ikke overskriv buffer 0 mens en sending venter.
//     if (CAN_Read(0x30) & (1 << 3)) { // TXB0CTRL.TXREQ
//         return 0;
//     }

//     // WRITE fra TXB0SIDH. CS holdes lav gjennom hele skrivingen.
//     SPI_Transfer(0x02, SPI_ID_CAN, 0); // WRITE
//     SPI_Transfer(0x31, SPI_ID_CAN, 0); // Startadresse

//     SPI_Transfer(0x24, SPI_ID_CAN, 0); // SIDH: ID 0x123 >> 3
//     SPI_Transfer(0x60, SPI_ID_CAN, 0); // SIDL: (ID & 7) << 5
//     SPI_Transfer(0x00, SPI_ID_CAN, 0); // EID8
//     SPI_Transfer(0x00, SPI_ID_CAN, 0); // EID0
//     SPI_Transfer(0x01, SPI_ID_CAN, 0); // DLC: én databyte
//     SPI_Transfer(0xFF, SPI_ID_CAN, 0); // Data

//     PORTB |= (1 << PB1_CAN_CS);

//     CAN_Request_To_Send2(0);

//     return 1; // Sending forespurt, ikke bekreftet fullført.
// }



void CAN_test(uint8_t mode){
    SPI_Transfer(CAN_resetregister, SPI_ID_CAN, 0);
     
    PORTB |= (1 << PB1_CAN_CS);

    _delay_ms(10);
    if(mode == 1){ 
    SPI_Transfer(0x40, SPI_ID_CAN,0); //Enables Loopback mode, For TESTING
    }else{
        //normal mode
    }
    PORTB |= (1 << PB1_CAN_CS);
    uint8_t canstat = CAN_Read(CAN_canstat);

    printf("canstat %d \n", canstat);

    if (canstat == 0x80) {
        printf("Svarer \n");
    } else {
        printf("Feil \n");
    }
}


// uint8_t CAN_read(uint8_t adr){

//     uint8_t value = 0;

//     SPI_Transfer(CAN_readregister, SPI_ID_CAN, 0);
//     SPI_Transfer(adr, SPI_ID_CAN, 0);
//     value = SPI_Transfer(0x00, SPI_ID_CAN, 0);
     
//     PORTB |= (1 << PB1_CAN_CS); //CS high

//     return value;

// }


// void CAN_reset() {

//     SPI_Transfer(CAN_resetregister, SPI_ID_CAN, 0);
     
//     PORTB |= (1 << PB1_CAN_CS);

//     _delay_ms(10);
// }


// void CAN_test(){
//     CAN_reset();
//     SPI_Transfer(0x40, SPI_ID_CAN,0); //Enables Loopback mode, For TESTING
//     PORTB |= (1 << PB1_CAN_CS);
//     uint8_t canstat = CAN_read(CAN_canstat);

//     printf("canstat %d \n", canstat);

//     if (canstat == 0x80) {
//         printf("Svarer \n");
//     } else {
//         printf("Feil \n");
//     }
// }

