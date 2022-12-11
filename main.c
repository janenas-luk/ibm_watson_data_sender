#include <stdio.h>
#include <signal.h>
#include <memory.h>
#include <stdlib.h>
#include <unistd.h>
#include <argp.h>
#include "watson.h"

volatile int interrupt = 0;

static struct argp_option options[] = { { "orgid", 'o', "organization", 0, "Organization id" },
					{ "typeid", 't', "type", 0, "Type id" },
					{ "deviceid", 'd', "device", 0, "Device id" },
					{ "authtoken", 'a', "token", 0, "Authorization token" },
					{ 0 } };

struct argp argp = { options, parse_opt, NULL, NULL };

/* Signal handler - to support CTRL-C to quit */
void sigHandler(int signo)
{
	signal(SIGINT, NULL);
	fprintf(stdout, "Received signal: %d\n", signo);
	interrupt = 1;
}

void hello(char *id, int rc, void *success, void *failure)
{
	printf("Event callback response for client: %s\n", id ? id : "");
}

/* Main program */
int main(int argc, char *argv[])
{
	int rc = 0;

	IoTPConfig *config	= NULL;
	IoTPDevice *device	= NULL;
	struct WatsonArgs wargs = { 0 };

	argp_parse(&argp, argc, argv, 0, 0, &wargs);
	// printf("Orgid %s\n", wargs.orgid);
	// printf("Typeid %s\n", wargs.typeid);
	// printf("Deviceid %s\n", wargs.deviceid);
	// printf("Authtoken %s\n", wargs.token);
	// exit(1);

	/* Set signal handlers */
	signal(SIGINT, sigHandler);
	signal(SIGTERM, sigHandler);

	rc = init_watson(&config, &device, &wargs);
	if (rc != 0) {
		// fprintf(stderr, "ERROR: Failed to initialize configuration: rc=%d\n", rc);
		goto watson_cleanup;
	}

	// set_watson_cbs(device, &hello);

	char *data = "{\"d\" : {\"SensorID\": \"Test\", \"Reading\": 15 }}";

	while (!interrupt) {
		// fprintf(stdout, "Send status event\n");
		rc = IoTPDevice_sendEvent(device, "status", data, "json", QoS0, NULL);
		// fprintf(stdout, "RC from publishEvent(): %d\n", rc);

		sleep(10);
	}

watson_cleanup:
	watson_cleanup(&config, &device);

	return 0;
}
