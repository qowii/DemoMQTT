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
#include <SPI.h>            //https://www.arduino.cc/en/reference/SPI

#include <WebSerialLite.h>
#include <FastLED.h>

/* Oracle Libraries */
#include <oracle_leds.h>
#include <oracle_leds_mqtt.h>
#include <oracle_mqtt.h>
#include <oracle_rc522.h>
#include <oracle_rc522_mqtt.h>
#include <oracle_hcsr04.h>
#include <oracle_utils_misc_mqtt.h>
#include <oracle_wifi.h>

/* Alice Libraries */
#include <AliceDefaultConfig.h>
#include <AliceWebSocketServer.h>
#include <version.h>

static const char *_alice_model_type = "RFID";
static AliceWebSocketServer *websocket = NULL;

void mqttCallback(char *topic, byte *payload, unsigned int length)
{
  if (oracle_leds_mqtt_callback(topic, payload, length))
    return;
}

void oracle_main_mqtt_init(void)
{
  oracle_mqtt_setup();
  oracle_mqtt_set_callback(mqttCallback);
}

void oracle_main_wait_for_upload(uint8_t seconds)
{
  char buffer[256];

  if (seconds == 0)
    return;

  snprintf(buffer, sizeof(buffer), "Upload :: Wait %d seconds for binary upload", seconds);
  Serial.println(buffer);
  WebSerial.println(buffer);

  delay(seconds * 1000);  /* Wait for WiFi to be ready */

  snprintf(buffer, sizeof(buffer), "Upload :: %d seconds elapsed", seconds);
  Serial.println(buffer);
  WebSerial.println(buffer);
}

void oracle_main_wait_for_upload(void)
{
  oracle_main_wait_for_upload(0);
}

void setup(void)
{
  char buffer[256];
  Serial.begin(115200);

  snprintf(buffer, sizeof(buffer), "Alice %s", _alice_model_type);
  Serial.println(buffer);

  aliceDumpBinaryInfo(false);
  Serial.println(_alice_model_type);
  
  oracle_leds_setup();
  oracle_leds_set_leds_color(CRGB::Red);

  oracle_wifi_setup();
  
  Serial.println("Starting Alice WebSocket");
  websocket = new AliceWebSocketServer(ALICE_SERVER_CONFIG_PORT);
  websocket->Run();
  delay(1000);

  oracle_main_wait_for_upload();

  oracle_main_mqtt_init();
  oracle_leds_set_leds_color(CRGB::Green);
  
  aliceDumpBinaryInfo(true);
  delay(1000); /* Wait for WiFi to be ready */

  oracle_rc522_setup();
  oracle_hcsr04_setup();
  
  oracle_leds_turn_leds_off();
}

void loop(void)
{
  oracle_utils_misc_mqtt_loop();
  websocket->loop();

  if (!oracle_wifi_loop())
    return;

  oracle_mqtt_loop();
  oracle_rc522_mqtt_loop();
}
