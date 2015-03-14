#include <stdlib.h>
#include <stdint.h>

#include <util/delay.h>

#include "repl.h"

void cmd_sleep(int argc, char **argv) {
	uint8_t s = 1;

	if(argc > 0)
		s = atoi(argv[1]);

	while(s--)
		_delay_ms(1000);
}

struct command core_cmds[] = {
	{"sleep", cmd_sleep},
	{NULL, NULL}
};
