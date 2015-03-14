#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <avr/pgmspace.h>
#include <avr/io.h>
#include <util/delay.h>

#include "repl.h"
#include "uart.h"
#include "readline.h"
#include "app.h"

extern struct command core_cmds[];
extern struct command cmds[];

static void skipws(char **p) {
	while(isspace(**p))
		(*p)++;
}

#define MAX_ARGS 8

static cmd_fn_t lookup_cmd(struct command *list, const char *name) {
	for(uint8_t i = 0; list[i].name; i++)
		if(!strcmp(list[i].name, name))
			return list[i].fn;

	return NULL;
}

static void execute(char *line) {
	char *argv[MAX_ARGS];
	int argc = 0;

	char *p = line;
	while(argc < MAX_ARGS) {
		skipws(&p);

		if(!*p)
			break;

		argv[argc] = p;
		argc++;

		while(*p && !isspace(*p))
			p++;

		if(!*p)
			break;

		*p = 0;
		p++;
	}

	if(argc == 0)
		return;

	cmd_fn_t fn = lookup_cmd(core_cmds, argv[0]);
	if(!fn)
		fn = lookup_cmd(cmds, argv[0]);

	if(!fn) {
		printf_P(PSTR("unknown command: %s\n\n"), argv[0]);
		printf_P(PSTR("available commands are:\n"));

		for(uint8_t i = 0; core_cmds[i].name; i++)
			printf_P(PSTR(" - %s (core)\n"), core_cmds[i].name);

		for(uint8_t i = 0; cmds[i].name; i++)
			printf_P(PSTR(" - %s\n"), cmds[i].name);

		return;
	}

	fn(argc, argv);
}

int main(void) {
	init_uart();
	init_readline();
	init_app();

	while(1) {
		char *line = readline("> ");

		skipws(&line);

		char *p;
		do {
			p = strchr(line, ';');
			if(p)
				*p = 0;

			execute(line);

			line = p + 1;
		} while(p);

		app_post();
	}
}
