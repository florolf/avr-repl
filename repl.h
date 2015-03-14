#pragma once

typedef void (*cmd_fn_t)(int argc, char **argv);

struct command {
	const char *name;
	cmd_fn_t fn;
};
