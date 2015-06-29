/*
 * mkuart.c
 *
 *  Created on: 2010-09-04
 *       Autor: Mirosław Kardaś
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <avr/pgmspace.h>

#include "mkuart.h"

void UART_Init( uint16_t _UBRR ) {
	/* Ustawienie pr�dko�ci */
	UBRRH = (uint8_t)(_UBRR>>8);
	UBRRL = (uint8_t)_UBRR;
	/* Za��czenie nadajnika I odbiornika */
	UCSRB = (1<<TXEN);
}


// definiujemy funkcj� dodaj�c� jeden bajtdoz bufora cyklicznego
void uart_putc( char data ) {

    while (!(UCSRA & (1<<UDRE)) ){}
    UDR = data;
}


void uart_puts(char *s) {
  while ( *s) uart_putc( *s++ );			// dop�ki nie napotkasz 0 wysy�aj znak
}

void uart_puts_P(const char *s) {	// wysy�a na port szeregowy tekst
	register char c;
	while ((c= pgm_read_byte( s++ ) )) uart_putc(c);
}

void uart_putint( uint32_t liczba, uint8_t radix ){
	char buf[17];
	ltoa (liczba, buf, radix);
	uart_puts(buf);
}
