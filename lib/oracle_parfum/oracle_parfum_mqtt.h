#ifndef __include_oracle_parfum_mqtt_h
#define __include_oracle_parfum_mqtt_h

#include <AliceDefaultConfig.h>
#include <oracle_parfum.h>

#define ORACLE_PARFUM_MQTT_LOOP_DELAY ALICE_ESP32_CONFIG_DELAY

/*!
 * @brief Parfum MQTT loop 
 */
bool oracle_parfum_mqtt_loop(void);

void oracle_parfum_mqtt_setup(void);
void oracle_parfum_mqtt_setup(oracle_parfum_config_t *config);

#endif /* #ifndef __include_oracle_parfum_mqtt_h */