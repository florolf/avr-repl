#pragma once

typedef void (*cmd_fn_t)(int argc, char **argv);

struct command {
	const char *name;
	cmd_fn_t fn;
};

cmd_fn_t lookup_cmd(struct command *list, const char *name);
