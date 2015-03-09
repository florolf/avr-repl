#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include <avr/io.h>

#include "uart.h"

void uart_putc_raw(uint8_t c) {
	while(!(UCSR0A & (1 << UDRE0)));

	UCSR0A |= 1 << TXC0; // clear TXC

	UDR0 = c;

	while(!(UCSR0A & (1 << UDRE0)) ||
			!(UCSR0A & (1 <<  TXC0)));
}

void uart_putc(uint8_t c) {
	if(c == '\n')
		uart_putc_raw('\r');

	uart_putc_raw(c);
}

uint8_t uart_getc_raw(void) {
	while(!(UCSR0A & (1 << RXC0)));

	uint8_t byte = UDR0;

	return byte;
}

uint8_t uart_getc(void) {
	uint8_t byte = uart_getc_raw();

	if(byte == '\r')
		byte = '\n';

	uart_putc(byte);

	return byte;
}

static int uputc(char c, FILE *f) {
	(void)f;

	uart_putc(c);

	return 0;
}

static int ugetc(FILE *f) {
	(void)f;

	return uart_getc();
}

void init_uart(void) {
	UBRR0H = 0;
	UBRR0L = 103;

	UCSR0A = (0 << U2X0);
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);

	fdevopen(uputc, ugetc);
}
