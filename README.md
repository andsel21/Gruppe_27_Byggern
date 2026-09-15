# Gruppe_27_Byggern
TTK4155 - Embedded Pong  Lab 


Her kan vi lage til README-info som nødvendig info for prosjektet  



UART - UCSRA - STATUS REGISTER
			BIT7 - RECEIVE COMPLETE - data er mottatt
			BIT6 - TRANSMIT COMPLETE - hele sendingen er ferdig
			BIT5 - DATA REGISTER EMPTY - UDR0 er klar for neste byte
			BIT4 - FRAME ERROR
			BIT3 - DATA OVERRUN
			BIT2 - PARITY ERROR
			BIT1 - Double the USART Transmission Speed
			BIT0 - MULTI-PROCESSOR COMMUNICATION MODE

UART - UCSRB - CONTROL REGISTER
			BIT7 - RX COMPLETE INTERRUPT ENABLE - interrupt når en byte er mottatt
			BIT6 - TX COMPLETE INTERRUPT ENABLE - interrupt når en hel UART-frame er ferdig sendt
			BIT5 - USART DATA REGISTER EMPTY INTERRUPT ENABLE - interrupt når UDR er ledig for neste byte
			BIT4 - RXEN: RECEIVER ENABLE
			BIT3 - TXEN: TRANSMIT ENABLE
			BIT2 - UCSZ2: CHARACTER SIZE - Brukes sammen med UCSZ1:0 for å velge antall databits
			BIT1 - RXB8: RECEIVE DATA BIT 8 - 9. mottatte databit ved 9-bit UART
			BIT0 - TXB8: TRANSMIT DATA BIT 8 - 9. databit som skal sendes ved 9-bit UART
			
UART - USCRC - Configuration / Frame Format
			BIT7 - URSEL - REGISTER SELECT
			BIT6 - UMSEL - USART MODE SELECT
			BIT5 - PARITY MODE BIT 1
			BIT4 - PARITY MODE BIT 0
			BIT3 - USBS - STOP BIT SELECT - 1 ELLER 2 STOPBITS
			BIT2 - UCSZ1 - CHARACTER SIZE BIT 1
			BIT1 - UCSZ2 - CHARACTER SIZE BIT 0
			BIT0 - UCPOL - CLOCK POLARITY - brukes ved synkron USART
