

#include "timeInformation.h"

const char* ssid     = "SetYourSSID";
const char* password = "SetYourPassword";
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;

#include <Esp.h>

byte setup_TimeInformation() {
    uint32_t flash_size = ESP.getFlashChipSize();
  
  Serial.print("Flash size: ");
  Serial.print(flash_size);
  Serial.println(" bytes");
    return setTimeInformation();
}

byte setTimeInformation() {

    #define RTC_UTC_TEST 1510592825 // 1510592825 = Monday 13 November 2017 17:07:05 UTC
  
    time_t rtc = RTC_UTC_TEST;
    timeval tv = { rtc, 0 };
    settimeofday(&tv, nullptr);

    /*Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    long tempMillis = millis();
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
        Serial.println(millis()-tempMillis);
        //if (millis()-tempMillis > 10000) { // 10 second wait
        //    Serial.println("Disconnect");
        //    return 101;
        //}
    }
    Serial.println("");
    Serial.println("WiFi connected.");

    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    Serial.println(getDate());
    Serial.println(getTime());

    //disconnect WiFi as it's no longer needed
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
    Serial.println("WIFI DISCONNECTED");*/
    return 0;
}

String getDate() {
    struct tm timeinfo;
    if(!getLocalTime(&timeinfo)){
        return "NOT FOUND";
    }

    char dateString[18];
    strftime(dateString, 18, "%a, %d. %b %Y", &timeinfo);
    return dateString;
}

String getTime() {
    struct tm timeinfo;
    if(!getLocalTime(&timeinfo)){
        return "NOT FOUND";
    }

    char timeString[6];
    strftime(timeString, 6, "%H:%M", &timeinfo);
    return timeString;
}

String getSpecificString(String strFormat, byte strLength) {
    struct tm timeinfo;
    if(!getLocalTime(&timeinfo)){
        return "NOT FOUND";
    }
    
    char timeString[strLength+1];
    strftime(timeString, strLength+1, strFormat.c_str(), &timeinfo);
    return timeString;
}


void setTime(tm &timeInfo) {
    time_t t = mktime(&timeInfo);
    struct timeval now = { .tv_sec = t };
    settimeofday(&now, NULL);
}

tm getCurrentTimeStruct() {
   struct tm timeinfo;
    if(!getLocalTime(&timeinfo)){
        return timeinfo;
    } 
    return timeinfo;
}
//EOF