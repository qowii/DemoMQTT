#ifndef __include_oracle_wifi_mqtt_h
#define __include_oracle_wifi_mqtt_h

#include <Arduino.h>
#include <AliceDefaultConfig.h>

#define ORACLE_WIFI_MQTT_LOOP_DELAY  ALICE_ESP32_CONFIG_DELAY

void oracle_wifi_mqtt_loop(void);
void oracle_wifi_mqtt_setup(void);

#endif /* #ifndef __include_oracle_mqtt_h */