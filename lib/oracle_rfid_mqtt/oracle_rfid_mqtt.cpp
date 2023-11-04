#include <PubSubClient.h> //Librairie pour la gestion Mqtt

#include "oracle_rfid_mqtt.h"

static oracle_screen_mqtt_callback_t screen_mqtt_callback = NULL;
static PubSubClient client(espClient);

static void oracle_screen_mqtt_callback(char* topic, byte *payload, unsigned int length)
{
    if (NULL == screen_mqtt_callback)
        return;

    mqtt_callback(topic, payload, length);
}

void oracle_screen_mqtt_publish(const char* topic, const char* payload)
{
    client.publish(topic, payload);
}

void oracle_screen_mqtt_publish_uuid(const char* payload)
{
    const char *topic = "/esp32/uuid";
    oracle_screen_mqtt_publish(topic, payload);
}

void oracle_screen_mqtt_reconnect(void)
{
    if (client.connected())
        return;

    Serial.println("Connection au serveur MQTT ...");
    if (client.connect("ESP32Client")) {
      Serial.println("MQTT connecté");
    }
    else {
      Serial.print("echec, code erreur= ");
      Serial.println(client.state());
    }
}

void oracle_screen_mqtt_set_callback(mqtt_callback_t callback)
{
    screen_mqtt_callback = callback;
}

void setup_mqtt(const char *mqtt_server, const int mqtt_port)
{
  client.setServer(mqtt_server, mqttPort);
  client.setCallback(oracle_screen_mqtt_callback);
  oracle_screen_mqtt_reconnect();
}