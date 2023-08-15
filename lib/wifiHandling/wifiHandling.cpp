#include "wifiHandling.h"

WiFiManager wm;

String ssid = "ABC";
String pass = "DEF";

boolean WIFIavailable;

void setup_WIFI() {
    Serial.println("Setup WIFI");
    WiFi.mode(WIFI_STA);
    if (wm.getWiFiIsSaved()) {
        ssid = wm.getWiFiSSID();
        pass = wm.getWiFiPass();
        if (WIFIconnect() != 0) {
            Serial.println("set wifi");
            WIFI_setNetwork();
        }
    } else {
        WIFI_setNetwork();
    }
    
}

//0 new wifi established
//1 no wifi available
byte WIFI_setNetwork() {
    //WIFIreset();
    wm.setConfigPortalTimeout(600);
    bool res = wm.autoConnect("AlarmClock", "password");
    if(!res) {
        Serial.println("Failed to connect");
        WIFIavailable = false;
        return 1;
    } else {  
        Serial.println("connected...yeey :)");
        WIFIavailable = true;
    }
    ssid = wm.getWiFiSSID();
    pass = wm.getWiFiPass();
    WIFIdisconnect();
    delay(500);
    return 0;
}

//0 conncetion works
//1 error, timeout
//10 wifi already established
byte WIFIconnect() {
    WIFIavailable = false;
    if (WiFi.status() != WL_CONNECTED) {
        WiFi.mode(WIFI_STA);
        delay(500);
        WiFi.begin(ssid, pass);
        delay(500);
        long tempMillis = millis();
        while (WiFi.status() != WL_CONNECTED && millis()-tempMillis < 10000) {
            delay(500);
        }
        if (WiFi.status() == WL_CONNECTED) {
            WIFIavailable = true;
            return 0;
        }
        WiFi.disconnect(false);
        return 1;
    }
    return 10;
}

void WIFIdisconnect() {
    WiFi.disconnect(false);
}

void WIFIreset() {
    wm.resetSettings();
}

//EOF