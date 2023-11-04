#ifndef AliceWiFi_h
#define AliceWiFi_h

#include <AliceDefaultConfig.h>

bool AliceWiFiStart(void);
bool AliceWiFiCheckConnectStatus(void);
void AliceWiFiWaitConnectStatus(void);
void AliceWiFiDumpInfos(void);
void aliceWiFiDetectDownConnection(int interval);

String AliceWiFiGetHostname(void);
String AliceWiFiGetMacAddress(void);
String AliceWiFiGetIpAdress(void);

#endif /* AliceWiFi_h */