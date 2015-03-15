#include <stdint.h>
#include <string.h>

#include "repl.h"

cmd_fn_t lookup_cmd(struct command *list, const char *name) {
	for(uint8_t i = 0; list[i].name; i++)
		if(!strcmp(list[i].name, name))
			return list[i].fn;

	return NULL;
}
