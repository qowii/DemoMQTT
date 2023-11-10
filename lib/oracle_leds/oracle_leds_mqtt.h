#ifndef __oracle_leds_mqtt_h
#define __oracle_leds_mqtt_h

#include <Arduino.h>

#include <oracle_config_default.h>
#include <oracle_leds.h>

#define ORACLE_LEDS_MQTT_LOOP_DELAY ORACLE_ESP32_CONFIG_DELAY

/*!
  * @brief Subscribe to leds topics
*/
void oracle_leds_mqtt_subscribe(void);

/*!
  * @brief Handle leds MQTT topics
  * @param topic MQTT topic
  * @param payload MQTT payload
  * @param length MQTT payload length
  * @return true if topic was handled, false otherwise
*/
bool oracle_leds_mqtt_callback(const char *topic, byte *payload, unsigned int length);

/*!
  * @brief Leds MQTT loop
*/
bool oracle_leds_mqtt_loop(void);

void oracle_leds_mqtt_setup(void);

#endif /* #ifndef __oracle_leds_mqtt_h */
