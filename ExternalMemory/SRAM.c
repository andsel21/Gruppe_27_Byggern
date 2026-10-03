/*
 * SRAM.c
 *
 * Created: 15.09.2026 15:06:19
 *  Author: andrksel
 */ 

/*
	External Memory: Address space: 0x500 <--> 0x84FF
	
			  XMEM Register
	
			Memory Config A
		AVR MCU				SRAM
	0x0500 - 0x7FFF -> 0x0500 - 0x7FFF
		AVR MCU				SRAM
	0x8000 -> 0x84FF -> 0x0000 - 0x04FF


				Memory Config B
		AVR MCU				SRAM
	0x0460 - 0x7FFF -> 0x0460 - 0x7FFF 
		AVR MCU				SRAM
	0x8000 - 0x845F -> 0x0000 - 0x045F

*/

/*
	PORTC 4 most significant bits on address buss = JTAG interface 
*/

/*
	When there is more than one unit on the bus, an address decoder
	will be necessary in order to choose which unit to be activated based on the address output on
	the address bus.
	
	DEMUX?
*/

/*
	Recommended Memory map: 0x1000 - 0x17FF og 0x1800 - 0x1FFF
	
	For enkelhetenskyld:
	ADC: 0x1000-0x17FF
	SRAM: 0x1800-0x1FFF
*/

// BRUK PC3 - A11 til Chip select SRAM har en aktiv hly og en aktiv lav signal. 
//S� PC3/A11 kan brukes til b�de ADC og SRAM chip.


#include "SRAM.h"
#define OFFSET 0x1800
#define AdcMemStart 0x1000
#define AdcMemEnd 0x17FF
#define SramMemStart 0x1800
#define SramMemEnd 0x1FFF
//#define addressPTR (*(volatile uint8_t *)) SramMemStart;

//volatile uint8_t *addressPTR = SramMemStart;

//struct MemoryReadWrite{
	//volatile uint8_t * PTR;
	//uint8_t package_size;
	//uint8_t package;
	//};

void InitSRAM(void){
	MCUCR |= (1<<SRE);
	
	//unsigned char *p = (unsigned char *)(OFFSET + 1);
	//DDRC = 0xFF;
	//PORTC = 0x00;
	SFIOR |= (1u<<XMM2);
	SFIOR &= ~(1u<<XMM1);
	SFIOR &= ~(1u<<XMM0);	
}

void WriteSRAM(struct MemoryReadWrite SRAMaddress){
		for(uint8_t i = 0;i < SRAMaddress.package_size; i++)
		{
			if(SRAMaddress.PTR >= 0x1800 && SRAMaddress.PTR <= (0x1FFF - SRAMaddress.package_size)){
			SRAMaddress.PTR[i] = SRAMaddress.package;
			}else{
				printf("Trying to write outside mem-area of SRAM");
			}
		}	
}

void ReadSRAM(struct MemoryReadWrite SRAMaddress, uint8_t ReadData[]){
		for (uint8_t i = 0; i < SRAMaddress.package_size;i++)
		{
			if(SRAMaddress.PTR >= 0x1800 && SRAMaddress.PTR <= (0x17FFF - SRAMaddress.package_size)){
			ReadData[i] = SRAMaddress.PTR[i];
			}else{
				printf("Trying to read outside mem-area of SRAM");
			}
		}
}




void SRAM_test(void)
{
	volatile char *ext_ram = (char *) 0x1800; // Start address for the SRAM
	uint16_t ext_ram_size = 0x800;
	uint16_t write_errors = 0;
	uint16_t retrieval_errors = 0;
	printf("Starting SRAM test ...\n") ;
	// rand() stores some internal state , so calling this function in a loop
	//will
	// yield different seeds each time (unless srand() is called before this
	//f unction )
	uint16_t seed = rand() ;
	// Write phase: Immediately check that the correct value was stored
	srand(seed) ;
	for (uint16_t i = 0; i < ext_ram_size; i++) {
		uint8_t some_value = rand();
		ext_ram[i] = some_value;
		uint8_t retreived_value = ext_ram[i];
		if(retreived_value != some_value) {
			printf("Write phase error : ext_ram[%4d] = %02X (should be %02X)\n" , i, retreived_value , some_value) ;
			write_errors++;
		}
	}
	// Retrieval phase: Check that no values were changed during or after the
	//write phase
	srand(seed) ;
	// reset the PRNG to the state it had before the write phase
	for (uint16_t i = 0; i < ext_ram_size; i++) {
		uint8_t some_value = rand();
		uint8_t retreived_value = ext_ram[i];
		if(retreived_value != some_value) {
			printf("Retrieval phase error : ext_ram[%4d] = %02X (should be %02X)\n" , i, retreived_value , some_value) ;
			retrieval_errors++;
		}
	}
	printf("SRAM test completed with \n%4d errors in write phase and \n%4d errors in retrieval phase\n\n" , write_errors , retrieval_errors) ;
}
