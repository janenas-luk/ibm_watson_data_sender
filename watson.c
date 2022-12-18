#include "watson.h"
#include <stdlib.h>
#include <string.h>

static volatile int run_loop = 1;

static int init_device(IoTPConfig *config, IoTPDevice **device)
{
	int rc = 0;
	rc     = IoTPDevice_create(device, config);
	if (rc != IOTPRC_SUCCESS) {
		return rc;
	}
	return rc;
}

static int init_config(IoTPConfig **config, struct WatsonArgs *args)
{
	int rc = 0;
	rc     = IoTPConfig_create(config, NULL);
	if (rc != IOTPRC_SUCCESS) {
		return rc;
	}
	IoTPConfig_setProperty(*config, "identity.orgId", args->orgid);
	if (rc != IOTPRC_SUCCESS) {
		return rc;
	}
	IoTPConfig_setProperty(*config, "identity.typeId", args->typeid);
	if (rc != IOTPRC_SUCCESS) {
		return rc;
	}
	IoTPConfig_setProperty(*config, "identity.deviceId", args->deviceid);
	if (rc != IOTPRC_SUCCESS) {
		return rc;
	}
	IoTPConfig_setProperty(*config, "auth.token", args->token);
	if (rc != IOTPRC_SUCCESS) {
		return rc;
	}
	return rc;
}

static void parse_option(struct WatsonArgs *args, char *option_line)
{
	char *token = strtok(option_line, " ");
	token	    = strtok(NULL, " ");
	if (strstr(option_line, "orgid") != NULL) {
		strcpy(args->orgid, token);
	} else if (strstr(option_line, "typeid") != NULL) {
		strcpy(args->typeid, token);
	} else if (strstr(option_line, "deviceid") != NULL) {
		strcpy(args->deviceid, token);
	} else if (strstr(option_line, "authtoken") != NULL) {
		strcpy(args->token, token);
	}
}

static int parse_config(struct WatsonArgs *args)
{
	FILE *fd = NULL;
	char line[128];
	fd = fopen(args->config, "r");
	if (fd == NULL) {
		return 1;
	}
	while (fgets(line, sizeof(line), fd)) {
		int len = strlen(line);
		if (len > 0 && line[len - 1] == '\n') {
			line[len - 1] = '\0';
		}
		parse_option(args, line);
	}
	fclose(fd);
	return 0;
}

int init_watson(IoTPConfig **config, IoTPDevice **device, struct WatsonArgs *args)
{
	int rc = 0;
	if (args->config) {
		rc = parse_config(args);
		if (rc) {
			return rc;
		}
	}
	rc = init_config(config, args);
	if (rc != IOTPRC_SUCCESS) {
		return rc;
	}
	rc = init_device(*config, device);
	if (rc != IOTPRC_SUCCESS) {
		return rc;
	}
	rc = IoTPDevice_connect(*device);
	return rc;
}

void set_watson_cbs(IoTPDevice *device, void (*eventCb)())
{
	if (eventCb != NULL) {
		IoTPDevice_setEventCallback(device, eventCb);
	}
}

void watson_cleanup(IoTPConfig **config, IoTPDevice **device)
{
	if (*device != NULL) {
		IoTPDevice_disconnect(*device);
		IoTPDevice_destroy(*device);
	}
	if (*config != NULL) {
		IoTPConfig_clear(*config);
	}
}