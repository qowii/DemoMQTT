#include <WiFi.h>
#include <PubSubClient.h> //Librairie pour la gestion Mqtt
#include <uptime_formatter.h>

#include <oracle_leds_mqtt.h>
#include <oracle_rfid_mqtt.h>

WiFiClient wlanclient;
static oracle_rfid_mqtt_callback_t oracle_rfid_mqtt_callback = NULL;
static PubSubClient oracle_rfid_mqtt_client(wlanclient);

static void _oracle_rfid_mqtt_callback(char* topic, byte *payload, unsigned int length)
{
    Serial.print ("Message arrived on Topic:");
    Serial.println(topic);

    Serial.println("receive msg !");

    if (NULL == oracle_rfid_mqtt_callback)
        return;

    oracle_rfid_mqtt_callback(topic, payload, length);
}

void oracle_rfid_mqtt_publish(const char* topic, const char* payload)
{
    // Retry twice
    if (!oracle_rfid_mqtt_client.publish(topic, payload)) {
        oracle_rfid_mqtt_reconnect();
        if (!oracle_rfid_mqtt_client.publish(topic, payload)) {
            Serial.print("Failed publish: ");
            Serial.print(strlen(topic));
            Serial.print(" ");
            Serial.println(strlen(payload));
        }
    }
}

void oracle_mqtt_print_payload(byte *payload, unsigned int length)
{
  Serial.print("payload: '");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.print("' (size: ");
  Serial.print(length);
  Serial.println(")");
}

void oracle_rfid_mqtt_publish_uptime(void)
{
    const char *topic = "/esp32/misc/get/uptime";
    String uptime = uptime_formatter::getUptimeWithMillis();
    oracle_rfid_mqtt_publish(topic, uptime.c_str());
}

void oracle_mqtt_subscribe(const char *topic)
{
    if (!oracle_rfid_mqtt_client.connected())
        return;

    oracle_rfid_mqtt_client.subscribe(topic);
}

void oracle_mqtt_misc_subscribe(void)
{
    if (!oracle_rfid_mqtt_client.connected())
        return;

    oracle_rfid_mqtt_client.subscribe("/esp32/misc/set/uptime");
}

void oracle_rfid_mqtt_reconnect(void)
{
    if (oracle_rfid_mqtt_client.connected())
        return;

    if (!oracle_rfid_mqtt_client.connect("ESP32Client")) {
      Serial.print("echec, code erreur= ");
      Serial.println(oracle_rfid_mqtt_client.state());
      return;
    }
    
    oracle_mqtt_misc_subscribe();
    oracle_leds_mqtt_subscribe();
}

void oracle_rfid_mqtt_set_callback(oracle_rfid_mqtt_callback_t callback)
{
    oracle_rfid_mqtt_callback = callback;
}

void oracle_rfid_mqtt_setup(const char *mqtt_server, const int mqtt_port)
{
    oracle_rfid_mqtt_client.setServer(mqtt_server, mqtt_port);
    oracle_rfid_mqtt_client.setCallback(_oracle_rfid_mqtt_callback);
    oracle_rfid_mqtt_reconnect();
}

void oracle_rfid_mqtt_loop(void)
{
    oracle_rfid_mqtt_client.loop();
}