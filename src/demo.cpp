/*
* Alice Models - =Rfid
* Rfid.ino
*
* Created by StudioOracle on 2023
*
* L'implémentation des .ino a fait l'objet d'une validation par le développeur.
* Toutefois, il est possible que des erreurs de syntaxe ou de logique aient été
* introduites lors de la modification de ces fichiers. Il est donc recommandé de
* vérifier le bon fonctionnement de l'application avant de la déployer sur un
* système embarqué. L'auteur se dégage de toute responsabilité en cas de
* dysfonctionnement de l'application sur un système embarqué.
*/

/* Arduino Libraries */
#include <Arduino.h>

/* Oracle Libraries */
#include <oracle_mqtt.h>
#include <oracle_wifi.h>

/* Component MQTT callback */
#include <oracle_screen_mqtt.h>
#include <oracle_hcsr04_mqtt.h>
#include <oracle_leds_mqtt.h>
#include <oracle_parfum.h>
#include <oracle_rc522_mqtt.h>
#include <oracle_utils_misc_mqtt.h>

/* Alice Libraries */
#include <AliceDefaultConfig.h>
#include <version.h>

static const char *_alice_model_type = "RFID";

void mqttCallback(char *topic, byte *payload, unsigned int length)
{
  if (oracle_leds_mqtt_callback(topic, payload, length))
    return;
}

void oracle_main_mqtt_setup(void)
{
  oracle_mqtt_setup();
  oracle_mqtt_set_callback(mqttCallback);
}

void setup(void)
{
  Serial.begin(115200);

  oracle_leds_mqtt_setup();

  oracle_leds_set_leds_color(CRGB::Red);
  oracle_leds_apply();

  oracle_wifi_setup();

  oracle_main_mqtt_setup();
  oracle_leds_set_leds_color(CRGB::Green);
  oracle_leds_apply();

  oracle_main_mqtt_setup();
  oracle_rc522_mqtt_setup();
  oracle_hcsr04_mqtt_setup();

  oracle_leds_turn_leds_off();
  oracle_leds_apply();
}

void loop(void)
{
  oracle_utils_misc_mqtt_loop();

  if (!oracle_wifi_loop())
    return;

  oracle_mqtt_loop();
  oracle_rc522_mqtt_loop();
}
