#include <uptime_formatter.h>

#include <oracle_mqtt.h>
#include <oracle_timer.h>
#include <oracle_utils_misc_mqtt.h>

static oracle_timer_loop_context_t oracle_timer_context;

static void oracle_utils_misc_mqtt_uptime(void)
{
    const char *topic = "misc/get/uptime";
    String uptime = uptime_formatter::getUptimeWithMillis();
    oracle_mqtt_publish(topic, uptime.c_str());
}

static void oracle_utils_misc_mqtt_esp32_type(void)
{
    const char *topic = "/esp32/misc/get/esp32_type";
    oracle_mqtt_publish(topic, ORACLE_UTILS_MISC_MQTT_ESP32_TYPE);
}

void oracle_utils_misc_mqtt_loop(void)
{
    oracle_timer_loop_context_t *timer_loop_ctx = &oracle_timer_context;

    if (!oracle_timer_loop_ready(timer_loop_ctx)) {
        oracle_utils_misc_mqtt_uptime();
        oracle_utils_misc_mqtt_esp32_type();
    }
}

void oracle_utils_misc_mqtt_subscribe(void)
{
    oracle_mqtt_subscribe("misc/set/restart");
}


void oracle_utils_misc_mqtt_setup(void)
{
    oracle_timer_loop_context_t *timer_loop_ctx = &oracle_timer_context;
    oracle_timer_loop_setup(timer_loop_ctx);
}