#ifndef __include_oracle_rfid_mqtt_h
#define __include_oracle_rfid_mqtt_h

#include <Arduino.h>

typedef void (*oracle_rfid_mqtt_callback_t) (char* topic, byte *payload, unsigned int length);

void oracle_rfid_mqtt_publish(const char* topic, const char* payload);
void oracle_screen_mqtt_publish_uuid(const char* payload);

void oracle_rfid_mqtt_reconnect(void);
void oracle_rfid_mqtt_set_callback(oracle_rfid_mqtt_callback_t callback);
void oracle_rfid_mqtt_setup(const char *mqtt_server, const int mqtt_port);

#endif /* #ifndef __include_oracle_rfid_mqtt_h */