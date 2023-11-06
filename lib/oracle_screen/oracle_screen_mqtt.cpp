
#include <Arduino.h>
#include <oracle_mqtt.h>
#include <oracle_utils.h>

#include <oracle_screen.h>
#include <oracle_screen_mqtt.h>

static void oracle_screen_mqtt_publish_state(uint8_t state)
{
    char buffer[16];
    const char *topic = "/esp32/screen/get/state";
    snprintf(buffer, 16, "%d", state);
    oracle_mqtt_publish(topic, buffer);
}

static bool oracle_screen_mqtt_set_state(const char *topic, byte *payload, unsigned int length)
{
    char *endptr;
    char buffer[256];
    uint8_t state;
    const char *expected_topic = "/esp32/screen/set/state";
    
    if (strncmp(topic, expected_topic, strlen(expected_topic)))
        return false;
    
    snprintf(buffer, sizeof(buffer), "%s", (char *) payload);

    state = oracle_utils_read_integer((char *)payload, &endptr);

    switch (state) {
        case ORACLE_SCREEN_ACTION_OPEN:
            oracle_screen_open();
            break;
        case ORACLE_SCREEN_ACTION_CLOSE:
            oracle_screen_close();
            break;
        case ORACLE_SCREEN_ACTION_STOP:
            oracle_screen_stop();
            break;
        default:
            break;
    }

    oracle_screen_mqtt_publish_state(state);
}

bool oracle_screen_mqtt_callback(const char *topic, byte *payload, unsigned int length)
{
    if (oracle_screen_mqtt_set_state(topic, payload, length))
        return true;

    return false;
}

bool oracle_screen_mqtt_loop(void)
{
    oracle_screen_loop();
}

void oracle_screen_mqtt_subscribe(void)
{
    oracle_mqtt_subscribe("/esp32/screen/set/state");
}

void oracle_screen_mqtt_setup(oracle_screen_config_t *config) {
    oracle_screen_setup(config);
}

void oracle_screen_mqtt_setup(void) {
    oracle_screen_setup();
}