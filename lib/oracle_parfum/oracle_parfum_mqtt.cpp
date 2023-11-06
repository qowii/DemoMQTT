#include <oracle_parfum.h>
#include <oracle_mqtt.h>
#include <oracle_utils.h>

static void oracle_parfum_publish_status(bool status)
{
    const char *topic = "/esp32/parfum/get/status";
    if (status) {
        oracle_mqtt_publish(topic, "true");
    } else {
        oracle_mqtt_publish(topic, "false");
    }
}

static void oracle_parfum_publish_duration(uint32_t duration)
{
    char payload[16];
    const char *topic = "/esp32/parfum/get/duration";
    sprintf(payload, "%d", duration);
    oracle_mqtt_publish(topic, payload);
}

static bool oracle_parfum_mqtt_set_status(const char *topic, byte *payload, unsigned int length)
{
    bool status;
    const char *expected_topic = "/esp32/parfum/set/status";
    
    if (strncmp(topic, expected_topic, strlen(expected_topic)))
        return false;
    
    status = oracle_utils_read_bool((char *)payload, length);
    if (status) {
        oracle_parfum_enable();
    } else {
        oracle_parfum_disable();
    }
    
    return true;
}

static bool oracle_parfum_mqtt_set_duration(const char *topic, byte *payload, unsigned int length)
{
    char *endptr;
    char buffer[256];
    uint32_t duration;
    const char *expected_topic = "/esp32/parfum/set/duration";
    
    if (strncmp(topic, expected_topic, strlen(expected_topic)))
        return false;
    
    snprintf(buffer, sizeof(buffer), "%s", (char *) payload);

    duration = oracle_utils_read_integer((char *)payload, &endptr);
    oracle_parfum_set_duration(duration);
    
    return true;
}

void oracle_parfum_mqtt_subscribe(void)
{
    oracle_mqtt_subscribe("/esp32/parfum/set/status");
    oracle_mqtt_subscribe("/esp32/parfum/set/delay");
}

bool oracle_parfum_mqtt_loop(void) {
  bool status;
  
  status = oracle_parfum_loop();
  oracle_parfum_publish_status(status);

  return true;
}

void oracle_parfum_mqtt_setup(oracle_parfum_config_t *config)
{
  oracle_parfum_setup(config);
}

void oracle_parfum_mqtt_setup(void)
{
  oracle_parfum_setup();
}