/*****************************************************************************
 *	    File: USARTLibrary.c
 *  Lab Name: (Needed for Lab07 and Lab08)
 *    Author: Dr. Greg Nordstrom
 *   Created: 3/3/2019
 *  Modified: Mandy Wells
 *  Mod Date: 10/20/2026
 * Processor: ATmega128A (on the ReadyAVR board)
 *
 * This is intended to be compiled as an AVR Library Project and
 * used by other programs. After compiling this code, copy the
 * libUSART1Library.a and USART1Library.h files to the project directory,
 * then add the library to the target project by R-clicking on the
 * project's Libraries directory and clicking Add Library. Also,
 * the USART1Library.h file must be #included in the project's .c file.
 *
 * NOTE: This library only works with the default USART1 port
 * on the ReadyAVR (so JP2 must have jumpers on PD3 and PD2).
 */

#include <avr/io.h>
#include "UARTLibrary.h"

// Use USART1_init to initialize the USART hardware in the AVR processor.
// The function takes the following arguments:
//    - desired baud rate (standard values from 2400 to 115200),
//    - number of databits (5-8),
//    - number of stop bits (1,2),
//    - parity (0=no parity, 1=odd parity, 2=even parity).
// No error checking is performed and the function does not return a value.
//
// For example,
//     usart1_init(9600,8,1,0);
// sets the baudrate to 9600, with 8 data bits, 1 stop bit, and no parity
void uart_init(long unsigned int baudrate, unsigned char databits, unsigned char stopbits, unsigned char parity) {
    /* your code here */
	unsigned int ubbr = ((7372800UL / (baudrate * 16UL)) - 1);
	
	// set baud rate (via UBRR0 high and low registers)
    UBRR1H = (unsigned char)(ubbr >> 8);
	UBRR1L = (unsigned char)ubbr;
	
	// set databits
	UCSR1B &= ~(1 << UCSZ12); // 0 for both 7 and 8
	if(databits == 7){
		UCSR1C |= (1 << UCSZ11);
		UCSR1C &= ~(1 << UCSZ10);
	}
	else{
		UCSR1C |= (1 << UCSZ11) | (1 << UCSZ10);
	}
    
	// set parity
	//00 = disabled, 10 = enabled + even, 11 = enabled + odd
	UCSR1C &= ~(1 << UPM10); 
	UCSR1C &= ~(1 << UPM11);
	
	if(parity == 1){
		UCSR1C |= (1 << UPM11) | (1 << UPM10);
	}
	else if(parity == 2){
		UCSR1C |= (1 << UPM11);
	}

    // set stopbits
	UCSR1C &= ~(1 << USBS1); // 0 for 1 stop bit, 1 for 2 stop bits
	
	if(stopbits == 2){
		UCSR1C |= (1 << USBS1);
	}
	
	// finally, enable receiver and transmitter
	// 1 enables the USARTm receiver and 1 enables the transmitter
	UCSR1B |= (1 << RXEN1) | (1 << TXEN1); //bit 4 = RXEN1, bit 3 = TXEN1
}

// This function waits for space in the USART transmit buffer. When space
// is available, the function places an unsigned character value (passed in
// as the function's only argument) in the buffer and returns. No error
// checking is performed and the function does not return a value.
void uart_tx(unsigned char data) {
    /* your code here */
	while(!(UCSR1A & (1 << UDRE1))){}
	UDR1 = data;
}

// This function blocks until a character is received in the USART input
// buffer. Once a character arrives, it is passed back to the caller as
// an unsigned character. No error checking is performed.
unsigned char uart_rx(void) {
	/* your code here */
    while(!(UCSR1A & (1 << RXC1))){}
	return UDR1;
}