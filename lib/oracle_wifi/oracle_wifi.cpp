#include <Arduino.h>
#include <FastLED.h>
#include <WiFi.h>

#include <oracle_wifi.h>
#include <oracle_leds.h>

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

static bool oracle_wifi_check_status(void)
{
    return (WL_CONNECTED == WiFi.status());
}

static String oracle_wifi_get_hostname(void)
{
    return WiFi.getHostname();
}

static void alice_wifi_dump_hostname(void)
{
    Serial.print("IP Address: ");
    Serial.println(oracle_wifi_get_hostname());
}

static String oracle_wifi_get_mac_address(void)
{
    return WiFi.macAddress();
}

static void alice_wifi_dump_mac_address(void)
{
    Serial.print("MAC Address: ");
    Serial.println(oracle_wifi_get_mac_address());
}

static String oracle_wifi_get_ip_address(void)
{
    return WiFi.localIP().toString();
}

static void alice_wifi_dump_ip_address(void)
{
    Serial.print("IP Address: ");
    Serial.println(oracle_wifi_get_ip_address());
}

void oracle_wifi_WiFiStationConnected_cb(WiFiEvent_t event, WiFiEventInfo_t info)
{
  Serial.println("\nConnected to AP successfully!");
}

static void oracle_wifi_WiFiGotIP_cb(WiFiEvent_t event, WiFiEventInfo_t info)
{
    Serial.println("\nWiFi connected");
    alice_wifi_dump_mac_address();
    alice_wifi_dump_ip_address();
}

static void oracle_wifi_WiFiStationDisconnected_cb(WiFiEvent_t event, WiFiEventInfo_t info)
{
  const char *ssid = ORACLE_RFID_WIFI_SSID;
  const char *password = ORACLE_RFID_WIFI_PWD;

  Serial.println("\nDisconnected from WiFi access point");
  Serial.print("WiFi lost connection. Reason: ");
  Serial.println(info.wifi_sta_disconnected.reason);
  Serial.println("Trying to Reconnect");

  WiFi.begin(ssid, password);
}

void oracle_wifi_setup(void)
{
    const char *ssid = ORACLE_RFID_WIFI_SSID;
    const char *password = ORACLE_RFID_WIFI_PWD;
    
    /* Turn on Wifi */
    Serial.println("Starting Alice WiFi");
    Serial.println();

    WiFi.mode(WIFI_STA);
    
    WiFi.onEvent(oracle_wifi_WiFiStationConnected_cb, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_CONNECTED);
    WiFi.onEvent(oracle_wifi_WiFiStationDisconnected_cb, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
    WiFi.onEvent(oracle_wifi_WiFiGotIP_cb, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP);

    WiFi.begin(ssid, password);
        
    Serial.println();
    Serial.println("Wait for WiFi... ");
}

void oracle_wifi_dump_info(void)
{
    if (!oracle_wifi_check_status()) {
        Serial.println("\nDisconnected from the WiFi network");
        return;
    } else {
        Serial.println("\nConnected to the WiFi network");
        alice_wifi_dump_hostname();
        alice_wifi_dump_mac_address();
        alice_wifi_dump_ip_address();
    }
}

bool oracle_wifi_loop(void)
{
  EVERY_N_MILLISECONDS(ORACLE_WIFI_LOOP_DELAY) {
    if (!oracle_wifi_check_status()) {
        oracle_leds_set_leds_color(CRGB::Red);
        return false;
    }
  }

  return true;
}