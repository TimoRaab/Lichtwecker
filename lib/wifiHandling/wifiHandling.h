#ifndef wifiHandling_h
#define wifiHandling_h

#include <WiFi.h>
#include <FS.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>
#include <WiFiManager.h>

void setup_WIFI();
void WIFI_setNetwork();
void WIFIconnect();
void WIFIdisconnect();
void WIFIreset();


#endif
//EOF