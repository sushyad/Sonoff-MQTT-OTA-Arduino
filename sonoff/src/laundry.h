#include "EmonLib.h"

void setupLaundrySensor();

boolean isDryerOn();

void reportDryerState(boolean isDryerOn);

void checkDryerStatus();

void mqtt_publish();
