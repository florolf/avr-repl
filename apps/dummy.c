#include <stdlib.h>
#include <stdint.h>
#include <util/delay.h>

#include "repl.h"
#include "io.h"
#include "readline.h"
#include "uart.h"

#include "app.h"

void init_app(void) {
}

void app_post(void) {
}

struct command cmds[] = {
	{NULL, NULL}
};
