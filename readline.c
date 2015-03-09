#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>

#include <avr/pgmspace.h>

#include "uart.h"
#include "readline.h"

#define MAX_LINE_LENGTH 128

#define CTRL(x) ((x) - 'A' + 1)
#define ESC 27
#define DEL 0x7f

uint8_t line_buffer[MAX_LINE_LENGTH];
uint8_t position;
uint8_t prompt_offset;
uint8_t line_length;

void init_readline(void) {
}

static void move_cursor(uint8_t col) {
	printf_P(PSTR("\033[%uG"), 1 + col + prompt_offset);

	position = col;
}

static void clear_line_after(void) {
	printf_P(PSTR("\033[K"));
}

// deletes from 'from' to before 'to'
static void delete_range(uint8_t from, uint8_t to) {
	if(to >= line_length)
		to = line_length - 1;

	memmove(line_buffer + from, line_buffer + to, line_length - to + 1); // +1 is for \0
	line_length -= to - from;

	uint8_t new_pos;
	if(position <= from)
		new_pos = position;
	else
		new_pos = position - (to - from);

	move_cursor(from);
	clear_line_after();
	printf("%s", (char*)(line_buffer + from));

	move_cursor(new_pos);
}

static uint8_t find_word(bool forward) {
	int p = position;
	int delta = forward ? 1 : -1;

	while(!isblank(line_buffer[p])) {
		if(p == 0 || p >= line_length)
			return p;

		p += delta;
	}

	while(isblank(line_buffer[p])) {
		if(p == 0 || p >= line_length)
			return p;

		p += delta;
	}

	if(!forward) {
		while(!isblank(line_buffer[p])) {
			if(p == 0 || p >= line_length)
				return p;

			p += delta;
		}

		p -= delta;
	}

	return p;
}

char *readline(const char *prompt) {
restart:
	memset(line_buffer, 0, MAX_LINE_LENGTH);
	line_length = 0;

	printf("%s", prompt);
	prompt_offset = strlen(prompt);

	move_cursor(0);

	while(1) {
		uint8_t c = uart_getc_raw();
		/*printf("\n");
		printf("0x%02X", c);
		if(isprint(c))
			printf(" (%c)", c);

		printf("\n");
		continue; */

		if(c == ESC) {
			c = uart_getc_raw();

			if(c != '[')
				continue;

			c = uart_getc_raw();
			switch(c) {
				case 'A': // up
					break;
				case 'B': // down
					break;
				case 'C': // right
					if(position < line_length)
						move_cursor(position + 1);
					break;
				case 'D': // left
					if(position > 0)
						move_cursor(position - 1);
					break;
				case 'b': // back word
					move_cursor(find_word(false));
					break;
				case 'f': // forward word
					move_cursor(find_word(true));
					break;
				case '3': // delete
					uart_getc_raw(); // discard tilde

					delete_range(position, position + 1);
					break;
				case '7': // home
					uart_getc_raw(); // discard tilde

					move_cursor(0);
					break;
				case '8': // end
					uart_getc_raw(); // discard tilde

					move_cursor(line_length);
					break;
			}

			continue;
		}

		switch(c) {
			case CTRL('A'): // home
				move_cursor(0);

				break;
			case CTRL('E'): // end
				move_cursor(line_length);

				break;
			case CTRL('C'): // abort
				putchar('\n');

				goto restart;
			case CTRL('U'): // clear
				move_cursor(0);
				clear_line_after();

				break;
			case CTRL('K'): // clear rest of line
				clear_line_after();

				break;
			case CTRL('H'): // backspace
			case DEL:
				if(position > 0)
					delete_range(position - 1, position);

				break;
			case CTRL('W'): // delete word
				delete_range(find_word(false), position);
				break;
			case CTRL('J'): // enter
			case '\r':
				uart_putc_raw('\r');
				uart_putc_raw('\n');
				return (char*)line_buffer;
			default: // insert self
				if(line_length >= MAX_LINE_LENGTH)
					break;

				uart_putc(c);

				line_buffer[line_length] = c;
				line_length++;
				position++;

				break;
		}
	}
}
