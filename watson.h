#include <iotp_device.h>
#include "args.h"

int init_watson(IoTPConfig **config, IoTPDevice **device, struct WatsonArgs *args);
void watson_cleanup(IoTPConfig **config, IoTPDevice **device);
void set_watson_cbs(IoTPDevice *device, void (*eventCb)());