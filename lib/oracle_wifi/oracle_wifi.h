#ifndef AliceWiFi_h
#define AliceWiFi_h

#include <Arduino.h>
#include <AliceDefaultConfig.h>

#define ORACLE_WIFI_LOOP_DELAY   ALICE_ESP32_CONFIG_DELAY
#define ORACLE_WIFI_HOSTNAME     ALICE_ESP32_CONFIG_NAME

//#define ORACLE_RFID_WIFI_CUSTOM

#ifdef ORACLE_RFID_WIFI_CUSTOM
#include "oracle_rfid_custom_newtork.h"
#define ORACLE_RFID_WIFI_SSID   ORACLE_RFID_WIFI_CUSTOM_SSID
#define ORACLE_RFID_WIFI_PWD    ORACLE_RFID_WIFI_CUSTOM_PWD
#else
#define ORACLE_RFID_WIFI_SSID   ALICE_WIFI_CONFIG_SSID
#define ORACLE_RFID_WIFI_PWD    ALICE_WIFI_CONFIG_PWD
#endif

void oracle_wifi_setup(void);
void oracle_wifi_dump_infos(void);

bool oracle_wifi_loop(void);

String oracle_wifi_get_ip_address(void);
String oracle_wifi_get_mac_address(void);
String oracle_wifi_get_hostname(void);

#endif /* AliceWiFi_h */