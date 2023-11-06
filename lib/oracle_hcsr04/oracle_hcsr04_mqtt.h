#ifndef __include_oracle_hcsr04_mqtt_h
#define __include_oracle_hcsr04_mqtt_h

#include <AliceDefaultConfig.h>

#define ORACLE_HCSR04_MQTT_LOOP_DELAY ALICE_ESP32_CONFIG_DELAY

/*!
 * @brief HCSR04 MQTT loop 
 */
bool oracle_hcsr04_mqtt_loop(void);

/*!
 * @brief HCSR04 MQTT subscribe
*/
void oracle_hcsr04_subscribe(void);

#endif /* __include_oracle_hcsr04_mqtt_h */