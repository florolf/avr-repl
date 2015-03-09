#pragma once

#include <stdint.h>

void init_uart(void);

uint8_t uart_getc_raw(void);
void uart_putc_raw(uint8_t c);

uint8_t uart_getc(void);
void uart_putc(uint8_t c);
