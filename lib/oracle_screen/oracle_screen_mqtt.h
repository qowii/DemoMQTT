#ifndef __include_oracle_screen_mqtt_h
#define __include_oracle_screen_mqtt_h

#include <Arduino.h>

#include <AliceDefaultConfig.h>
#include <oracle_screen.h>

#define ORACLE_SCREEN_MQTT_LOOP_DELAY ALICE_ESP32_CONFIG_DELAY

typedef enum oracle_screen_action_e {
    ORACLE_SCREEN_ACTION_OPEN,
    ORACLE_SCREEN_ACTION_CLOSE,
    ORACLE_SCREEN_ACTION_STOP
} oracle_screen_action_t;

/*!
  * @brief Subscribe to screen topics
*/
void oracle_screen_mqtt_subscribe(void);

/*!
  * @brief Handle screen MQTT topics
  * @param topic MQTT topic
  * @param payload MQTT payload
  * @param length MQTT payload length
  * @return true if topic was handled, false otherwise
*/
bool oracle_screen_mqtt_callback(const char *topic, byte *payload, unsigned int length);

/*!
  * @brief screen MQTT loop
*/
bool oracle_screen_mqtt_loop(void);

void oracle_screen_mqtt_setup(void);
void oracle_screen_mqtt_setup(oracle_screen_config_t *config);

#endif /* #ifndef __include_oracle_screen_mqtt_h */