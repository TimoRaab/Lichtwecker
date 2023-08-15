#ifndef wifiHandling_h
#define wifiHandling_h

#include <WiFi.h>
#include <FS.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>
#include <WiFiManager.h>

void setup_WIFI();
byte WIFI_setNetwork();
byte WIFIconnect();
void WIFIdisconnect();
void WIFIreset();


extern boolean WIFIavailable;


#endif
//EOF