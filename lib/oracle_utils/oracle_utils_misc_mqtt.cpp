#include <FastLED.h>
#include <uptime_formatter.h>

#include <oracle_mqtt.h>
#include <oracle_utils_misc_mqtt.h>

static void oracle_utils_misc_mqtt_uptime(void)
{
    const char *topic = "/esp32/misc/get/uptime";
    String uptime = uptime_formatter::getUptimeWithMillis();
    oracle_mqtt_publish(topic, uptime.c_str());
}

void oracle_utils_misc_mqtt_loop(void)
{
    EVERY_N_SECONDS(ORACLE_UTILS_MISC_MQTT_UPTIME_DELAY) {
        oracle_utils_misc_mqtt_uptime();
    }
}