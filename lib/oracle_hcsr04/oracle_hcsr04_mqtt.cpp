#include <oracle_mqtt.h>
#include <oracle_hcsr04.h>
#include <oracle_hcsr04_mqtt.h>

/*!
 * @brief Publish the distance to the MQTT broker
 * @param distance The distance to publish
 */
static void oracle_hcsr04_mqtt_publish_distance(uint8_t distance)
{
    char buffer[16];
    const char *topic = "/esp32/hcsr04/get/dist";
    snprintf(buffer, sizeof(buffer), "%u", distance);
    oracle_mqtt_publish(topic, buffer);
}

bool oracle_hcsr04_mqtt_loop(void)
{
    uint8_t distance;

    distance = oracle_hcsr04_loop();
    if ( ORACLE_HCSR04_SKIP_VALUE != distance) {
        oracle_hcsr04_mqtt_publish_distance(distance)
    }

    return true;
}

void oracle_hcsr04_subscribe(void)
{
    return;
}

void oracle_hcsr04_mqtt_setup(oracle_hcsr04_config_t *config){
    oracle_hcsr04_setup(config);
}

void oracle_hcsr04_mqtt_setup(void)
{
    oracle_hcsr04_setup();
}