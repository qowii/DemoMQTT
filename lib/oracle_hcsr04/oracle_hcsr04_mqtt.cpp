
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

void oracle_hcsr04_mqtt_loop(void)
{
    const uint8_t dist = oracle_hcsr04_get_distance();
    oracle_hcsr04_mqtt_publish_distance(dist);
}

void oracle_hcsr04_subscribe(void)
{
    return;
}