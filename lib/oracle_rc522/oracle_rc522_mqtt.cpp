#include <oracle_rc522.h>
#include <oracle_mqtt.h>

static void oracle_rc522_mqtt_publish_uuid(oracle_rc522_uuid_t *uuid)
{
    char uuid_str[16];
    const char *topic = "/esp32/rfid/get/uuid";
    oracle_rc522_copy_uuid(uuid, uuid_str, 16);
    oracle_mqtt_publish(topic, uuid_str);
}

bool oracle_rc522_mqtt_loop(void)
{
    oracle_rc522_uuid_t new_uuid;

    if (!oracle_rc522_read(&new_uuid)) {
      return false;
    }

    oracle_rc522_mqtt_publish_uuid(&new_uuid);
    return true;
}

void oracle_rc522_mqtt_setup(void)
{
  oracle_rc522_setup();
}

void oracle_rc522_mqtt_setup(oracle_rc522_config_t *config)
{
  oracle_rc522_setup(config);
}