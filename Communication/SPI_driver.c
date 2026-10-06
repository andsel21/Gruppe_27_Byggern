
/*
 * SPI.c
 *
 * Created: 03.10.2026 16:28:59
 *  Author: andrksel
 */ 

#include "SPI_driver.h"
#include <stdio.h>

#define SPI_SS PB4
#define SPI_MOSI PB5
#define SPI_MISO PB6
#define SPI_SCK PB7

//Distributed Shift Register

//			MCU					 SLAVE
//	    Shift Register		Shift Register
//

//Clock Phase 
// CPHA: 0/1
// CPOL: 0/1

//MODE 0 : CPOL = 0 og CPHA = 0
//MODE 1 : CPOL = 0 og CPHA = 1
//MODE 2 : CPOL = 1 og CPHA = 0
//MODE 3 : CPOL = 1 og CPHA = 1

//SPI STATUS REGISTER: SPIF, WCOL, SPI2X

//SPI CONTROL REGISTER - SPCR [SPIE, SPE, DORD,MSTR,CPOL,CPHA,SPR1,SPR0]


void Init_SPI(void){
	//Sets MOSI,SCK and SS as outputs. MISO remains an input
	DDRB |= (1 << SPI_MOSI) |(1 << SPI_MISO) | (1 << SPI_SCK) | (1 << SPI_SS);
	
	//SPE = 1 (Enable), MSTR = 1 (MASTER), SPR1:0=00 (F_CPU/4)
	//CPOL = 0, CPHA = 0 (MODE 0)
	SPCR = (1 << SPE) | (1 << MSTR);
	//SPSR: Ensure no double speed
	SPSR &= ~(1 << SPI2X);
	//Setting Clock-rate fosc/64
	SPCR &= ~(1<<SPR0);
	SPCR |= (1<<SPR1); 
}


uint8_t SPI_Transfer(uint8_t byte, SPI_CS_t CS, uint8_t D_C){
	
	if(CS == SPI_ID_CONTROLLER){ //N�r man sender data ut p� MOSI, vil OLED returnere samme data p� MISO.
		PORTB |= (1 << PB3); // Deaktiverer OLED SLAVE cHIP
		PORTB &= ~(1<<PB4); //Aktiver Slave chip, deaktiver andre senere
		SPDR = byte;
		while(!(SPSR & (1 << SPIF))); //Wait for transmission complete
		PORTB |= (1<<PB4); // deaktiverer etter melding
		if(SPDR != byte){
			printf("DIFFERENCE!! \n"); //Kan lages som til error-funksjon`?
		}
		
		return SPDR;
	}

	if(CS == SPI_ID_OLED){
		PORTB |= (1 << PB4); // Deaktiverer CONTROLLER SLAVE cHIP
		PORTB &= ~(1<<PB3); //Aktiver Slave chip, deaktiver andre senere
		if(D_C){
			PORTB |= (1<<PB2); //D/C# PIN	
		}
		SPDR = byte;
		while(!(SPSR & (1 << SPIF)));
		// PORTB |= (1<<PB3);  // Deaktivere etter melding
		PORTB &= ~(1<<PB2); // D/C# PIN
		if(SPDR != byte){
			printf("OLED!! \n"); //Kan lages som til error-funksjon`?
		}
		return SPDR;
	}
	
	if(CS == SPI_ID_CAN){
		PORTB |= (1<<PB4); //Settes OLED CS H�y for deaktiver
		//M� ogs� deaktiver SS3, n�r den tid kjem
		//DO DIS
		return SPDR;
	}
	
	if(CS == SS3){
		//DO DIS
			return SPDR;
	}
	//NB : Chip must be selected prior to call
	//SPDR = byte;
	//while(!(SPSR & (1 << SPIF))){ //Wait for transmission complete
}


