
#include <FastLED.h>

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
    EVERY_N_MILLISECONDS(ORACLE_HCSR04_MQTT_LOOP_DELAY) {
        const uint8_t dist = oracle_hcsr04_get_distance();
        oracle_hcsr04_mqtt_publish_distance(dist);
    }

    return true;
}

void oracle_hcsr04_subscribe(void)
{
    return;
}