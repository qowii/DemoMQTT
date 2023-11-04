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
#include <ArduinoJson.h>
#include <WebSerialLite.h>
#include <FastLED.h>
#include <uptime_formatter.h>

//#include "oracle_rfid_leds.h"
#include "oracle_rfid_rc522.h"

/* Alice Libraries */
#include <AliceDefaultConfig.h>
#include <AliceWebSocketServer.h>
#include <AliceWiFi.h>
#include <version.h>

typedef enum rfid_action_type {
  RFID_ACTION_TYPE_INVALID = 0,
  RFID_ACTION_TYPE_EMPTY = 1,
  RFID_ACTION_TYPE_POLL = 2,
  RFID_ACTION_TYPE_IDLE = 3,
  RFID_ACTION_TYPE_COUNT
} rfid_action_type_e;

typedef enum rfid_status_type {
  RFID_STATUS_TYPE_INVALID = 0,
  RFID_STATUS_TYPE_IDLE = 1,
  RFID_STATUS_TYPE_ACTIVE = 2,
  RFID_STATUS_TYPE_COUNT
} rfid_status_type_e;

typedef enum rfid_error_type {
  RFID_ERROR_TYPE_OK = 0,
  RFID_ERROR_TYPE_BUSY = 1,
  RFID_ERROR_TYPE_INVALID = 2,
  RFID_ERROR_TYPE_COUNT
} rfid_error_type_e;

static const char *rfidAction2String[] = {
  "RFID_ACTION_INVALID",
  "RFID_ACTION_EMPTY",
  "RFID_ACTION_POLL",
  "RFID_ACTION_IDLE"
};

static const char *rfidStatus2String[] = {
  "RFID_STATUS_INVALID",
  "RFID_STATUS_IDLE",
  "RFID_STATUS_ACTIVE"
};

static const char *rfidError2String[] = {
  "RFID_ERROR_OK",
  "RFID_ERROR_BUSY",
  "RFID_ERROR_INVALID"
};

typedef struct rfid_state {
  uint8_t status;
  uint8_t error;
  uint8_t action;
  uint8_t pad[5];
} rfid_state_t;

typedef struct rfid_options {
  char uuid[32];
} rfid_options_t;

typedef struct rfid_context {
  rfid_state_t state;
  rfid_options_t options;
  SemaphoreHandle_t mutex;
} rfid_context_t;

static const char *_alice_model_type = "RFID";
static AliceWebSocketServer *websocket = NULL;
static rfid_context_t __rfid_context;
static bool rfid_is_valid = false;

static void rfidLock(void) {
  xSemaphoreTake(__rfid_context.mutex, portMAX_DELAY);
}

static void rfidUnlock(void) {
  xSemaphoreGive(__rfid_context.mutex);
}

int RfidIsEmpty(void)
{
  char uuid_empty_str[64];
  char *uuid_str = __rfid_context.options.uuid;
  oracle_rfid_rc522_copy_uuid(&oracle_rfid_rc522_empty_uuid, uuid_empty_str, 16);
  return !strncmp(__rfid_context.options.uuid, uuid_empty_str, 16);
}

void resetRfidHex(void)
{
  char *uuid_str = NULL;
  rfidLock();
  uuid_str = __rfid_context.options.uuid;
  oracle_rfid_rc522_copy_uuid(&oracle_rfid_rc522_empty_uuid, uuid_str, 16);
  //oracle_rfid_leds_turnoff();
  rfid_is_valid = false;
  rfidUnlock();
}

void setRfidHex(oracle_rfid_rc522_uuid_t *new_uuid)
{
  char *uuid_str = NULL;

  rfidLock();
  uuid_str = __rfid_context.options.uuid;
  oracle_rfid_rc522_copy_uuid(new_uuid, __rfid_context.options.uuid, 16);
  //oracle_rfid_leds_change_color(CRGB::Blue);
  rfid_is_valid = true;
  rfidUnlock();
}


static void rfidContextInit(void)
{
  char *uuid_str = __rfid_context.options.uuid;
  //memset(&__rfid_context, 0, sizeof(__rfid_context));
  __rfid_context.state.error = RFID_ERROR_TYPE_OK;
  __rfid_context.state.action = RFID_ACTION_TYPE_POLL;
  __rfid_context.state.status = RFID_STATUS_TYPE_ACTIVE;
  oracle_rfid_rc522_copy_uuid(&oracle_rfid_rc522_empty_uuid, uuid_str, 16);
  __rfid_context.mutex = xSemaphoreCreateMutex();
}

static void rfidContextFini(void)
{
  vSemaphoreDelete(__rfid_context.mutex);
}

static const char *rfidActionGetName(const uint8_t index)
{
  if (index >= RFID_ACTION_TYPE_COUNT)
    return rfidAction2String[RFID_ACTION_TYPE_INVALID];

  return rfidAction2String[index];
}

static const char *rfidStatusGetName(const uint8_t index)
{
  if (index >= RFID_STATUS_TYPE_COUNT)
    return rfidStatus2String[RFID_STATUS_TYPE_INVALID];

  return rfidStatus2String[index];
}

static const char *rfidErrorGetName(const uint8_t index)
{
  if (index < 0 || index >= RFID_ERROR_TYPE_COUNT)
    return rfidError2String[RFID_ERROR_TYPE_INVALID];
  return rfidError2String[index];
}

static void updateCurrentState(JsonDocument& jsonRequest)
{
  const char *action = jsonRequest["action"];

  if (NULL == action) {
    rfidLock();
    __rfid_context.state.error = RFID_ERROR_TYPE_INVALID;
    rfidUnlock();
    return;
  }

  /* Just Answer with current status */
  if (!strcmp(action, "status")) {
    return;
  }

  if (!strcmp(action, "idle") || !strcmp(action, "reset")) {;
    return;
  }
}

void handleWebSocketMessage(JsonDocument& jsonRequest)
{
  JsonObject response;
  rfid_context_t current_ctx;

  updateCurrentState(jsonRequest);

  rfidLock();
  memcpy(&current_ctx, &__rfid_context, sizeof(rfid_context_t));
  rfidUnlock();

  /* Generic data */
  response = jsonRequest.to<JsonObject>(); /* Get empty document */
  response["status"] = rfidStatusGetName(current_ctx.state.status);
  response["action"] = rfidActionGetName(current_ctx.state.action);
  response["error"]  = rfidErrorGetName(current_ctx.state.error);

  /* options */
  response.createNestedObject("options");
  response["options"]["UUID"] = current_ctx.options.uuid; 

  resetRfidHex();
}

void setup(void)
{
  char buffer[256];
  Serial.begin(115200);

  snprintf(buffer, sizeof(buffer), "Alice %s", _alice_model_type);
  Serial.println(buffer);

  aliceDumpBinaryInfo(false);
  Serial.println(_alice_model_type);

  rfidContextInit();
  
  //oracle_rfid_leds_init();
  //oracle_rfid_leds_change_color(CRGB::Red);

  /* Turn on Wifi */
  Serial.println("Starting Alice WiFi");
  Serial.println();
  AliceWiFiStart();

  //oracle_rfid_leds_change_color(CRGB::Blue);
  AliceWiFiWaitConnectStatus();
  
  Serial.println("Starting Alice WebSocket");
  websocket = new AliceWebSocketServer(ALICE_SERVER_CONFIG_PORT);
  websocket->SetHandleMessageCallback(handleWebSocketMessage);
  websocket->Run();
  delay(1000);

  Serial.println("Setup network DONE");
  WebSerial.println("Setup network DONE");

  //oracle_rfid_leds_change_color(CRGB::Green);
  
  Serial.println("Upload :: Wait 15 seconds for binary upload");
  WebSerial.println("Wait 15 seconds for binary upload");
  delay(15 * 1000); /* Wait for WiFi to be ready */
  Serial.println("Upload :: 15 seconds elapsed");
  WebSerial.println("Upload :: 15 seconds elapsed");

  aliceDumpBinaryInfo(true);
  delay(1000); /* Wait for WiFi to be ready */

  oracle_rfid_rc522_config_t rfid_rc522_config = ORACLE_RFID_RC522_CONFIG_DEFAULT();
  oracle_rfid_rc522_dump_config(&rfid_rc522_config);
  //oracle_rfid_rc522_init(&rfid_rc522_config);

  //oracle_rfid_leds_turnoff();
}

static void processAction(void)
{ 

  oracle_rfid_rc522_uuid_t new_uuid;
  
  if (!oracle_rfid_rc522_read(&new_uuid))
    return;
  
  setRfidHex(&new_uuid);
}

void loop(void)
{
  EVERY_N_SECONDS(60) {
        Serial.println("up " + uptime_formatter::getUptime());
  }

  EVERY_N_MILLISECONDS(ALICE_ESP32_CONFIG_DELAY) {
    websocket->cleanupClients();
    if (!AliceWiFiCheckConnectStatus()) {
      //oracle_rfid_leds_change_color(CRGB::Red);
    } 
    else 
    {
      processAction();
      
      if (RfidIsEmpty()) {
        //oracle_rfid_leds_turnoff();        
      } else {
        //oracle_rfid_leds_change_color(CRGB::Blue);
      }
    }
  }
}