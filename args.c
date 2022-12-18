#include "args.h"
#include <string.h>

error_t parse_opt(int key, char *arg, struct argp_state *state)
{
	struct WatsonArgs *arguments = state->input;

	switch (key) {
	case 'o':
		strcpy(arguments->orgid, arg);
		break;
	case 't':
		strcpy(arguments->typeid, arg);
		break;
	case 'd':
		strcpy(arguments->deviceid, arg);
		break;
	case 'a':
		strcpy(arguments->token, arg);
		break;
	case 'c':
		strncpy(arguments->config, arg, 127);
		break;
	default:
		return ARGP_ERR_UNKNOWN;
	}
	return 0;
}