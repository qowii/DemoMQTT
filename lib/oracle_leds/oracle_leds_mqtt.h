#ifndef __oracle_leds_mqtt_h
#define __oracle_leds_mqtt_h

#include "Arduino.h"

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

#endif /* #ifndef __oracle_leds_mqtt_h */
