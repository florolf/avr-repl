#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <avr/io.h>
#include <util/delay.h>

#include "uart.h"
#include "readline.h"

struct {
	const char *name;
	void (*fn)(char *arg);
} cmds[] = {
	{NULL, NULL}
};

int main(void) {
	init_uart();
	init_readline();

	while(1) {
		char *line = readline("> ");
		printf("read line '%s'\n", line);
	}
}
