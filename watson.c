#include "watson.h"

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

int init_watson(IoTPConfig **config, IoTPDevice **device, struct WatsonArgs *args)
{
	int rc = 0;
	rc     = init_config(config, args);
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
	IoTPDevice_disconnect(*device);
	IoTPDevice_destroy(*device);
	IoTPConfig_clear(*config);
}