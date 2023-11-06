#include <FastLED.h>
#include <PubSubClient.h> //Librairie pour la gestion Mqtt
#include <WiFi.h>

#include <oracle_mqtt.h>
#include <oracle_leds_mqtt.h>

WiFiClient wlanclient;
static oracle_mqtt_callback_t oracle_mqtt_callback = NULL;
static PubSubClient oracle_mqtt_client(wlanclient);

static void _oracle_mqtt_callback(char* topic, byte *payload, unsigned int length)
{
    Serial.print ("Message arrived on Topic:");
    Serial.println(topic);

    Serial.println("receive msg !");

    if (NULL == oracle_mqtt_callback)
        return;

    oracle_mqtt_callback(topic, payload, length);
}

void oracle_mqtt_publish(const char* topic, const char* payload)
{
    // Retry twice
    if (!oracle_mqtt_client.publish(topic, payload)) {
        oracle_mqtt_reconnect();
        if (!oracle_mqtt_client.publish(topic, payload)) {
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

void oracle_mqtt_subscribe(const char *topic)
{
    if (!oracle_mqtt_client.connected())
        return;

    oracle_mqtt_client.subscribe(topic);
}

void oracle_mqtt_misc_subscribe(void)
{
    if (!oracle_mqtt_client.connected())
        return;

    oracle_mqtt_client.subscribe("/esp32/misc/set/uptime");
}

void oracle_mqtt_reconnect(void)
{
    if (oracle_mqtt_client.connected())
        return;

    if (!oracle_mqtt_client.connect("ESP32Client")) {
      Serial.print("echec, code erreur= ");
      Serial.println(oracle_mqtt_client.state());
      return;
    }
    
    oracle_mqtt_misc_subscribe();
    oracle_leds_mqtt_subscribe();
}

void oracle_mqtt_set_callback(oracle_mqtt_callback_t callback)
{
    oracle_mqtt_callback = callback;
}

bool oracle_mqtt_loop(void)
{
    EVERY_N_MILLISECONDS(ORACLE_MQTT_LOOP_DELAY) {
        oracle_mqtt_client.loop();
    }

    return true;
}

void oracle_mqtt_setup(oracle_mqtt_config_t *config)
{
    oracle_mqtt_client.setServer(config->mqtt_server, config->mqtt_port);
    oracle_mqtt_client.setCallback(_oracle_mqtt_callback);
}

void oracle_mqtt_setup(void)
{
    oracle_mqtt_config_t config = ORACLE_MQTT_CONFIG_DEFAULT();
    oracle_mqtt_setup(&config);
}