#include <AsyncElegantOTA.h>
#include <FastLED.h>
#include <WebSerialLite.h>
#include "AliceWebSocketServer.h"

#define ALICE_WEBSOCKET_MAX_MSG_SIZE 256
static handle_msg_fn _handle_msg_cb = NULL;
static StaticJsonDocument<256> jsonData;
static char message[ALICE_WEBSOCKET_MAX_MSG_SIZE+1];
static char buffer[ALICE_WEBSOCKET_MAX_MSG_SIZE+1];

enum AliceWbsocketMessageType {
  ALICE_WEBSOCKET_MESSAGE_TYPE_UNDEFINED,
  ALICE_WEBSOCKET_MESSAGE_TYPE_PING,
  ALICE_WEBSOCKET_MESSAGE_TYPE_TEXT,
  ALICE_WEBSOCKET_MESSAGE_TYPE_COUNT
};

void print_json_document_memory_usage(void)
{
  if (jsonData.memoryUsage() > 1000) {
    Serial.print("Memory usage: ");
    Serial.println(jsonData.memoryUsage());
    serializeJsonPretty(jsonData, Serial);
  }
}

/* Message callback of WebSerial */
static void recvMsg(uint8_t *data, size_t len)
{
  DeserializationError error;
  
  print_json_document_memory_usage();

  error = deserializeJson(jsonData, data, len);
  if (error) {
    snprintf(buffer, sizeof(buffer), "deserializeJson() failed: %s", error.c_str());
    WebSerial.println(buffer);
    return;
  }

  print_json_document_memory_usage();

  serializeJson(jsonData, buffer, sizeof(buffer));
  WebSerial.println(buffer);

  if (NULL != _handle_msg_cb && !jsonData["action"].isNull()) {
    _handle_msg_cb(jsonData);
  }

  serializeJson(jsonData, buffer, ALICE_WEBSOCKET_MAX_MSG_SIZE);
  WebSerial.println(buffer);
}

static uint8_t msgIsValid(void *arg, size_t len)
{
  AwsFrameInfo *info = (AwsFrameInfo*) arg;

  if (info->opcode != WS_TEXT) {
    return ALICE_WEBSOCKET_MESSAGE_TYPE_UNDEFINED;
  }

  if (info->index != 0)
    return ALICE_WEBSOCKET_MESSAGE_TYPE_UNDEFINED;

  if (info->len != len)
    return ALICE_WEBSOCKET_MESSAGE_TYPE_UNDEFINED;

  if (!info->final)
    return ALICE_WEBSOCKET_MESSAGE_TYPE_UNDEFINED;

  return ALICE_WEBSOCKET_MESSAGE_TYPE_TEXT;
}

static uint8_t jsonSyntaxCheck(void *arg, uint8_t *data, size_t len,
                               JsonDocument& jsonData)
{
  uint8_t msg_type;

  /* Check message infos */
  msg_type = msgIsValid(arg, len);

  if (msg_type != ALICE_WEBSOCKET_MESSAGE_TYPE_TEXT) {
    return ALICE_WEBSOCKET_MESSAGE_TYPE_UNDEFINED;
  }

  /* Deserialize Client Message */
  if (msg_type == ALICE_WEBSOCKET_MESSAGE_TYPE_TEXT) {
    DeserializationError error;
    error = deserializeJson(jsonData, data, len);
    if (error) {
      WebSerial.print("deserializeJson() failed: ");
      WebSerial.println(error.c_str());
      return ALICE_WEBSOCKET_MESSAGE_TYPE_UNDEFINED;
    }
  }

  return msg_type;
}

static void echoMsg(AsyncWebSocket *server, JsonDocument& jsonData)
{
  size_t size_to_copy;

  size_to_copy = measureJson(jsonData) + 1;
  if (size_to_copy >= ALICE_WEBSOCKET_MAX_MSG_SIZE)
    size_to_copy = ALICE_WEBSOCKET_MAX_MSG_SIZE - 1;

  print_json_document_memory_usage();

  serializeJson(jsonData, message, size_to_copy);
  message[size_to_copy] = '\0';

  jsonData.clear();
  print_json_document_memory_usage();

  /* All clients received datas */
  server->textAll(message);
}

static void handleWsMsg(AsyncWebSocket *server,
                        void *arg, uint8_t *data, size_t len)
{
  uint8_t msg_type;

  msg_type = jsonSyntaxCheck(arg, data, len, jsonData);

  if (msg_type != ALICE_WEBSOCKET_MESSAGE_TYPE_TEXT)
    return;

  /* Process Client Command */
  if (NULL != _handle_msg_cb && !jsonData["action"].isNull()) {
      _handle_msg_cb(jsonData);
  }

  echoMsg(server, jsonData);
}

static void onWsEvent(AsyncWebSocket *server,
                      AsyncWebSocketClient *client,
                      AwsEventType type, void *arg,
                      uint8_t *data, size_t len)
{
  switch (type) {
    case WS_EVT_CONNECT:
    case WS_EVT_DATA:
      //WebSerial.println("Websocket client connection received");
      handleWsMsg(server, arg, data, len);
      break;

    case WS_EVT_DISCONNECT:
      //WebSerial.println("Websocket client disconnection received");
      break;

    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

AliceWebSocketServer::AliceWebSocketServer(int port)
{
    _port = port;
    _server = new AsyncWebServer(_port);
    _socket = new AsyncWebSocket("/ws");
}

AliceWebSocketServer::AliceWebSocketServer(void)
{
    AliceWebSocketServer(ALICE_WEBSOCKET_SERVER_DEFAULT_PORT);
}

AsyncWebServer *AliceWebSocketServer::getServer(void)
{
  return _server;
}

void AliceWebSocketServer::cleanupClients(void)
{
  _socket->cleanupClients();
}

void AliceWebSocketServer::loop(void)
{
  EVERY_N_MILLISECONDS(ORACLE_WS_SERVER_LOOP_DELAY) {
    this->cleanupClients();
  }
}

void AliceWebSocketServer::SetHandleMessageCallback(handle_msg_fn func)
{
  _handle_msg_cb = func;
}

void AliceWebSocketServer::Run(void)
{
  AsyncElegantOTA.begin(_server);

  WebSerial.onMessage(recvMsg);
  WebSerial.begin(_server);

  _socket->onEvent(onWsEvent);
  _server->addHandler(_socket);
  _server->begin();
}