#ifndef AliceWebSocketServer_h
#define AliceWebSocketServer_h

#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>
#include <AliceDefaultConfig.h>

#define ORACLE_WS_SERVER_LOOP_DELAY ALICE_ESP32_CONFIG_DELAY

typedef void (*handle_msg_fn)(JsonDocument&);

class AliceWebSocketServer {
    public:
        AliceWebSocketServer(int value);
        AliceWebSocketServer(void);
        void cleanupClients(void);
        void SetHandleMessageCallback(handle_msg_fn func);
        void Run(void);
        AsyncWebServer *getServer(void);
        void loop(void);

    private:
        AsyncWebServer *_server;
        AsyncWebSocket *_socket;
        SemaphoreHandle_t _mutex;
        int _port;
};


#endif /* AliceWebSocketServer_h */