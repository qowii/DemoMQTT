#include <oracle_rc522.h>
#include <oracle_rfid_mqtt.h>

static void oracle_rfid_mqtt_publish_uuid(oracle_rfid_rc522_uuid_t *uuid)
{
    char uuid_str[16];
    const char *topic = "/esp32/rfid/get/uuid";
    oracle_rfid_rc522_copy_uuid(uuid, uuid_str, 16);
    oracle_rfid_mqtt_publish(topic, uuid_str);
}

void oracle_rc522_mqtt_loop(void)
{

    oracle_rfid_rc522_uuid_t new_uuid;

    if (!oracle_rfid_rc522_read(&new_uuid)) {
      return;
    }

    oracle_rfid_mqtt_publish_uuid(&new_uuid); 
}