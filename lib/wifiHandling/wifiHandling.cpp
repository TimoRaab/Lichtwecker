#include "wifiHandling.h"

WiFiManager wm;

String ssid = "ABC";
String pass = "DEF";

void setup_WIFI() {
    Serial.println("Setup WIFI");
    WiFi.mode(WIFI_STA);
    WIFI_setNetwork();
}

void WIFI_setNetwork() {
    //WIFIreset();
    wm.setConfigPortalTimeout(600);
    bool res = wm.autoConnect("AlarmClock", "password");
    if(!res) {
        Serial.println("Failed to connect");
    } else {  
        Serial.println("connected...yeey :)");
    }
    ssid = wm.getWiFiSSID();
    pass = wm.getWiFiPass();
    WIFIdisconnect();
    delay(500);
}

void WIFIconnect() {
    if (WiFi.status() != WL_CONNECTED) {
        WiFi.mode(WIFI_STA);
        delay(500);
        Serial.print("SSID: ");
        Serial.println(wm.getWiFiSSID());
        Serial.print("Pass: ");
        Serial.println(wm.getWiFiPass());
        WiFi.begin(ssid, pass);
        delay(500);
        long tempMillis = millis();
        while (WiFi.status() != WL_CONNECTED && millis()-tempMillis < 10000) {
            delay(500);
        }
    }
}

void WIFIdisconnect() {
    WiFi.disconnect(false);
}

void WIFIreset() {
    wm.resetSettings();
}

//EOF