#ifndef __include_oracle_mqtt_h
#define __include_oracle_mqtt_h

#include <Arduino.h>

#define ORACLE_MQTT_DEFAULT_PORT 1883
#define ORACLE_MQTT_DEFAULT_SERVER "192.168.1.140"

#define ORACLE_MQTT_CONFIG_DEFAULT() {\
    .mqtt_server = ORACLE_MQTT_DEFAULT_SERVER,\
    .mqtt_port = ORACLE_MQTT_DEFAULT_PORT\
}

typedef struct oracle_mqtt_config_s {
    const char *mqtt_server;
    const int mqtt_port;
} oracle_mqtt_config_t;

typedef void (*oracle_mqtt_callback_t) (char* topic, byte *payload, unsigned int length);

void oracle_mqtt_subscribe(const char *topic);
void oracle_mqtt_publish(const char* topic, const char* payload);
void oracle_mqtt_publish_uptime(void);

void oracle_mqtt_reconnect(void);
void oracle_mqtt_set_callback(oracle_mqtt_callback_t callback);

void oracle_mqtt_setup(void);
void oracle_mqtt_setup(oracle_mqtt_config_t *config);

void oracle_mqtt_loop(void);

#endif /* #ifndef __include_oracle_mqtt_h */