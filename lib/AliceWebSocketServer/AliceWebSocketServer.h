#ifndef AliceWebSocketServer_h
#define AliceWebSocketServer_h

#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>
#include <AliceDefaultConfig.h>

typedef void (*handle_msg_fn)(JsonDocument&);

class AliceWebSocketServer {
    public:
        AliceWebSocketServer(int value);
        AliceWebSocketServer(void);
        void cleanupClients(void);
        void SetHandleMessageCallback(handle_msg_fn func);
        void Run(void);
        AsyncWebServer *getServer(void);

    private:
        AsyncWebServer *_server;
        AsyncWebSocket *_socket;
        SemaphoreHandle_t _mutex;
        int _port;
};

#endif /* AliceWebSocketServer_h */