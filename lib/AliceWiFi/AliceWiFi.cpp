
#include <WiFi.h>

#include "AliceWiFi.h"
#include <esp_wifi.h>

// ALICE_WIFI_CONFIG_MAC_ADDRESS_DEFAULT
static uint8_t AliceWifiNewMACAddress[6] = {0x32, 0xAE, 0xA4, 0x07, 0x0D, 0x66};

const char *AliceWiFiStatus2String(wl_status_t status) {
  switch (status) {
    case WL_NO_SHIELD: return "WL_NO_SHIELD";
    case WL_IDLE_STATUS: return "WL_IDLE_STATUS";
    case WL_NO_SSID_AVAIL: return "WL_NO_SSID_AVAIL";
    case WL_SCAN_COMPLETED: return "WL_SCAN_COMPLETED";
    case WL_CONNECTED: return "WL_CONNECTED";
    case WL_CONNECT_FAILED: return "WL_CONNECT_FAILED";
    case WL_CONNECTION_LOST: return "WL_CONNECTION_LOST";
    case WL_DISCONNECTED: return "WL_DISCONNECTED";
  }
    return "WL_UNKNOWN";
}

bool AliceWiFiCheckConnectStatus(void)
{
    return (WL_CONNECTED == WiFi.status());
}

void AliceWiFiWaitConnectStatus(void)
{
    int elapsed = 0;

    while(1) {
        if (AliceWiFiCheckConnectStatus())
            break;

        //Serial.print(".");
        delay(ALICE_WIFI_CONFIG_TIMEOUT);
    }
}

void AliceWifiParseMacAddressString(const char *macAddressString)
{
    return;
}

void AliceWifiSetMacAddress(void)
{
    int errno;
    uint8_t readMacAddr[6];
    Serial.print("[OLD] ESP32 Board MAC Address:  ");
    Serial.println(WiFi.macAddress());
/*     const char *macAddress = ALICE_WIFI_CONFIG_MAC_ADDRESS;
    AliceWifiParseMacAddressString(macAddress); */
    
    errno = esp_wifi_set_mac(WIFI_IF_STA, AliceWifiNewMACAddress);
    if(errno != ESP_OK) {
      Serial.print("Failed to change mac address ");
      Serial.println(esp_err_to_name(errno));
    }
  
    errno = esp_wifi_get_mac(WIFI_IF_STA, readMacAddr);
    if(errno != ESP_OK) {
      Serial.print("Failed to read mac address ");
      Serial.println(esp_err_to_name(errno));
    }

    Serial.print("[NEW] ESP32 Board MAC Address:  ");
    Serial.println(WiFi.macAddress());
}

void AliceWifiSetHostname(void)
{
    const char *hostname = ALICE_ESP32_CONFIG_NAME;
    WiFi.setHostname(hostname);
}

String AliceWiFiGetHostname(void)
{
    return WiFi.getHostname();
}

String AliceWiFiGetMacAddress(void)
{
    return WiFi.macAddress();
}

String AliceWiFiGetIpAdress(void)
{
    return WiFi.localIP().toString();
}

static void AliceWiFiPrintIpAdress(void)
{
    Serial.print("IP Address: ");
    Serial.println(AliceWiFiGetIpAdress());
}

static void AliceWiFiPrintMacAdress(void)
{
    Serial.print("MAC Address: ");
    Serial.println(AliceWiFiGetMacAddress());
}

static void AliceWiFiPrintHostname(void)
{
    Serial.print("Hostname Address: ");
    Serial.println(AliceWiFiGetHostname());
}

static void AliceWiFiScanNetwork(void){

    int n = WiFi.scanNetworks();
    Serial.println("scan done");
  
    if (n == 0 || n > 10) {
        Serial.println("no networks found");
    } else {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i) {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*");
      delay(10);
    }
  }

  Serial.println("");
}

void WiFiStationConnected(WiFiEvent_t event, WiFiEventInfo_t info)
{
  Serial.println("\nConnected to AP successfully!");
}

void WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info)
{
    Serial.println("\nWiFi connected");
    AliceWiFiPrintMacAdress();
    AliceWiFiPrintIpAdress();
}

void WiFiStationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info)
{
  const char *ssid = ALICE_WIFI_CONFIG_SSID;
  const char *password = ALICE_WIFI_CONFIG_PWD;

  Serial.println("\nDisconnected from WiFi access point");
  Serial.print("WiFi lost connection. Reason: ");
  Serial.println(info.wifi_sta_disconnected.reason);
  Serial.println("Trying to Reconnect");

  WiFi.begin(ssid, password);
}

bool AliceWiFiStart(void)
{
    const char *ssid = ALICE_WIFI_CONFIG_SSID;
    const char *password = ALICE_WIFI_CONFIG_PWD;
    
    WiFi.mode(WIFI_STA);

    AliceWifiSetHostname();
    //WiFi.disconnect(true);

    delay(1000);

    WiFi.onEvent(WiFiStationConnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_CONNECTED);
    WiFi.onEvent(WiFiGotIP, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP);
    WiFi.onEvent(WiFiStationDisconnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);

    WiFi.begin(ssid, password);
        
    Serial.println();
    Serial.println("Wait for WiFi... ");

    return true;
}

void AliceWiFiDumpInfos(void)
{
    if (!AliceWiFiCheckConnectStatus()) {
        Serial.println("\nDisconnected from the WiFi network");
        AliceWiFiPrintMacAdress();
        return;
    } else {
        Serial.println("\nConnected to the WiFi network");
        AliceWiFiPrintIpAdress();
        AliceWiFiPrintMacAdress();
        AliceWiFiPrintHostname();
    }
}

void aliceWiFiDetectDownConnection(int interval)
{
    static unsigned long previousMillis;
    unsigned long currentMillis = millis();

    // if WiFi is down, try reconnecting every CHECK_WIFI_TIME seconds
    if ((WiFi.status() != WL_CONNECTED) && (currentMillis - previousMillis >= interval)) {
        Serial.print(millis());
        Serial.println("Reconnecting to WiFi...");
        WiFi.disconnect();
        WiFi.reconnect();
        previousMillis = currentMillis;
    }
}
