#include "laundry.h"
#include "debug.h"
#include "base.h"

EnergyMonitor dryer;                   // Create an instance

boolean lastTimeDryerWasOn = false;
boolean wasDryerOn = false;
boolean dryerIsOn = false;
boolean isInitialized = false;
int repeatCount = 0;

long currentTime, lastMessageTime = 0;

void setupLaundrySensor() {
  dryer.current(A0, 30);
  //lastMessageTime = millis();
}

boolean isDryerOn() {
  char svalue[6];
  double irms_dryer = dryer.calcIrms(1480);  // Calculate Irms only
  double powerUsage = irms_dryer*230.0;  // Apparent power
  dtostrf(powerUsage, 4, 0, svalue);
  addLog(LOG_LEVEL_DEBUG, svalue);
  //mqtt_publish("wsn/washer/powerusage", svalue);
  return (powerUsage > 1800.0);
}

void reportDryerState() {
  if (dryerIsOn != lastTimeDryerWasOn) {
    lastTimeDryerWasOn = dryerIsOn;

    if (dryerIsOn) {
      mqtt_publish("wsn/dryer/status", "ON");
     } else {
      mqtt_publish("wsn/dryer/status", "OFF");
    }
  }
}

void checkDryerStatus() {
  currentTime = millis();

  if (currentTime - lastMessageTime > 10000) {
    boolean dryerOn = isDryerOn();

    if (!isInitialized) {
      isInitialized = true;
    }

    if (dryerOn == wasDryerOn) {
      repeatCount++;

      if (repeatCount >= 3) {
        dryerIsOn = dryerOn;
        reportDryerState();
      }
    } else {
      repeatCount = 0;
    }

    wasDryerOn = dryerOn;

    lastMessageTime = currentTime;
  }
}
