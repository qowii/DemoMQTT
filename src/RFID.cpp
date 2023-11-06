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

#include <ArduinoJson.h>
#include <WebSerialLite.h>
#include <FastLED.h>
#include <uptime_formatter.h>

/* Oracle Libraries */
#include "oracle_leds.h"
#include "oracle_leds_mqtt.h"
#include "oracle_mqtt.h"
#include "oracle_rc522.h"
#include "oracle_rc522_mqtt.h"
#include "oracle_hcsr04.h"

/* Alice Libraries */
#include <AliceDefaultConfig.h>
#include <AliceWebSocketServer.h>
#include <AliceWiFi.h>
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

  /* Turn on Wifi */
  Serial.println("Starting Alice WiFi");
  Serial.println();
  AliceWiFiStart();

  oracle_leds_set_leds_color(CRGB::Blue);
  AliceWiFiWaitConnectStatus();
  
  Serial.println("Starting Alice WebSocket");
  websocket = new AliceWebSocketServer(ALICE_SERVER_CONFIG_PORT);
  websocket->Run();
  delay(1000);

  oracle_main_mqtt_init();

  Serial.println("Setup network DONE");
  WebSerial.println("Setup network DONE");

  oracle_leds_set_leds_color(CRGB::Green);
  
  Serial.println("Upload :: Wait 15 seconds for binary upload");
  WebSerial.println("Wait 15 seconds for binary upload");
  //delay(15 * 1000); /* Wait for WiFi to be ready */
  Serial.println("Upload :: 15 seconds elapsed");
  WebSerial.println("Upload :: 15 seconds elapsed");

  aliceDumpBinaryInfo(true);
  delay(1000); /* Wait for WiFi to be ready */

  oracle_rc522_init();

  oracle_hcsr04_setup();

  oracle_leds_turn_leds_off();
}

void loop(void)
{
  EVERY_N_SECONDS(60) {
    oracle_mqtt_publish_uptime();
  }

  EVERY_N_MILLISECONDS(ALICE_ESP32_CONFIG_DELAY) {
    
    if (!AliceWiFiCheckConnectStatus()) {
      oracle_leds_set_leds_color(CRGB::Red);
      return;
    }
  
    oracle_mqtt_loop();
    oracle_rc522_mqtt_loop();
    websocket->cleanupClients();
  }
}
