
#include <Arduino.h>

/* oracle libraries */
#include <oracle_mqtt.h>
#include <oracle_timer.h>
#include <oracle_wifi.h>
#include <oracle_wifi_mqtt.h>

static oracle_timer_loop_context_t oracle_timer_context;

static void oracle_wifi_mqtt_publish_hostname(void)
{
    const char *topic = "/esp32/wifi/get/hostname";
    String hostname = oracle_wifi_get_hostname();
    oracle_mqtt_publish(topic, hostname.c_str());
}

static void oracle_wifi_mqtt_publish_mac_address(void)
{
    const char *topic = "/esp32/wifi/get/mac_address";
    String mac_address = oracle_wifi_get_mac_address();
    oracle_mqtt_publish(topic, mac_address.c_str());
}

static void oracle_wifi_mqtt_publish_ip_address(void)
{
    const char *topic = "/esp32/wifi/get/ip_address";
    String ip_address = oracle_wifi_get_ip_address();
    oracle_mqtt_publish(topic, ip_address.c_str());
}

void oracle_wifi_mqtt_loop(void)
{
    oracle_timer_loop_context_t *timer_loop_ctx = &oracle_timer_context;

    if (!oracle_timer_loop_ready(timer_loop_ctx)) {
        oracle_wifi_mqtt_publish_hostname();
        oracle_wifi_mqtt_publish_mac_address();
        oracle_wifi_mqtt_publish_ip_address();
    }
}

void oracle_wifi_mqtt_subscribe(const char *topic)
{
    return;
}

void oracle_wifi_mqtt_setup(void)
{
    oracle_timer_loop_context_t *timer_loop_ctx = &oracle_timer_context;

    oracle_timer_loop_setup(timer_loop_ctx);
    oracle_wifi_setup();
}



