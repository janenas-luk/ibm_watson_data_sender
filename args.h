#include <argp.h>

struct WatsonArgs {
	char orgid[25];
	char typeid[25];
	char deviceid[25];
	char token[25];
	char config[128];
};

error_t parse_opt(int key, char *arg, struct argp_state *state);