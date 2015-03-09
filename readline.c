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

uint8_t line_buffer[MAX_LINE_LENGTH];
uint8_t position;
uint8_t prompt_offset;
uint8_t line_length;

void init_readline(void) {
}

static void move_cursor(uint8_t col) {
	printf_P(PSTR("\033[%uG"), col + prompt_offset);

	position = col;
}

static void clear_line(void) {
	puts_P(PSTR("\033[K"));

	memset(line_buffer, 0, MAX_LINE_LENGTH);
	line_length = 0;
}

static void delete_to(uint8_t idx) {
	if(idx >= line_length)
		idx = line_length - 1;

	uint8_t a, b;
	if(idx < position) {
		a = idx;
		b = position;

		move_cursor(a);
	} else {
		a = position;
		b = idx;
	}

	memmove(line_buffer + a, line_buffer + b, line_length - b);

	line_length -= b - a;
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
	clear_line();
	position = 0;

	puts(prompt);
	prompt_offset = strlen(prompt);

	while(1) {
		uint8_t c = uart_getc();

		if(c == ESC) {
			c = uart_getc();

			if(c != '[')
				continue;

			c = uart_getc();
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
					uart_getc(); // discard tilde

					delete_to(position + 1);
					break;
				case '7': // home
					uart_getc(); // discard tilde

					move_cursor(0);
					break;
				case '8': // end
					uart_getc(); // discard tilde

					move_cursor(line_length - 1);
					break;
			}

			continue;
		}

		switch(c) {
			case CTRL('A'): // home
				move_cursor(0);

				break;
			case CTRL('E'): // end
				move_cursor(line_length - 1);

				break;
			case CTRL('C'): // abort
				putchar('\n');

				goto restart;
			case CTRL('U'): // clear
				move_cursor(0);
				clear_line();

				break;
			case CTRL('K'): // clear rest of line
				clear_line();

				break;
			case CTRL('H'): // backspace
				delete_to(position - 1);

				break;
			case CTRL('W'): // delete word
				delete_to(find_word(false));
				break;
			case CTRL('J'): // enter
				line_buffer[line_length - 1] = 0;

				return (char*)line_buffer;
			default: // insert self
				line_buffer[line_length] = c;
				line_length++;

				break;
		}
	}
}
