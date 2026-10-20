/*****************************************************************************
 *	    File: Lab08.c
 *  Lab Name: RS232 Communication Server
 *    Author: Dr. Greg Nordstrom
 *   Created: 12/30/17
 *  Mod Date: 3/28/19, 1/17/21
 *  Modified: Mandy Wells
 *   ModDate: 10/20/2026
 * 
 * Processor: ATmega128A (on the ReadyAVR board)
 *
 * This program implements a communication server. Normally the server
 * simply echoes what was sent. However, there are two cases where
 * different behavior occurs:
 *    1. Pressing the ESC causes a soft reset (redisplays the title text)
 *    2. Typing the letters 'end' contiguously displays 'Bye' and halts
 *
 *****************************************************************************/

// define CPU speed (7.3728 MHz for the ATmega128A)
#define F_CPU   7372800UL

#define CR          0x0D
#define LF          0x0A
#define ESC         0x1B

#include <avr/io.h>
#include <util/delay.h>
#include "UARTLibrary.h"  // Double quotes for local library.

#define BAUDRATE 1200 // standard values from 1200 to 115200
#define DATABITS    8 // 7 or 8
#define STOPBITS    1 // 1,2
#define PARITY      0 // 0=none, 1=odd, 2=even

/* Your code here.  */
void uart_print(char *str)
{
	while (*str != '\0')
	{
		uart_tx(*str);
		str++;
	}
}

void show_status(void){
	uart_print("\r\n\r\n");
	uart_print("RS232 Communications Server\r\n");
	uart_print("Status: Active\r\n");
	uart_print("Mandy Wells\r\n");
	uart_print("EECE3624\r\n");
	uart_print("Fall 2023\r\n\r\n");
}

int main(void){
	/* Your code here.  */
	unsigned char c, count = 0;
	
	uart_init(BAUDRATE, DATABITS, STOPBITS, PARITY);
	
	DDRA |= (1 << PA0);
	PORTA |= (1 << PA0);
	
	show_status();

    /* See the UARTLibrary.h for available calls.
	 *     	  // initialize the UART
	 *     	  uart_init(BAUDRATE, DATABITS, STOPBITS, PARITY);
	 *
	 *        // transmit a value (0x41) to the comm port
	 *        c = 0x41;
	 *        uart_tx(c);
	 *
	 *        // receive a byte from the comm port
	 *        c = uart_rx();
	 */

	while(1)
	{
		/* your loop code here */
		c = uart_rx(); 
		PORTA &= ~(1 << PA0); // LED0 on
		_delay_ms(75); 
		PORTA |= (1 << PA0); // LED0 off 
		
		if (c == 0x1B) { // Esc: soft reset 
			show_status(); 
			count = 0; 
			continue; 
		} 
		
		uart_tx(c); // echo 
		if (c == '\r') uart_tx('\n'); // CR also sends LF 
		
		// "end" detection 
		if (c == 'e')						count = 1;
		else if (c == 'n' && count == 1)	count = 2; 
		else if (c == 'd' && count == 2)	count = 3; 
		else								count = 0; 
		
		if (count == 3) {
			uart_print("\r\n\r\n"); 
			uart_print("Bye\r\n"); 
			while (1) {} // stop until hard reset 
		}
	}
	return 0;
}
