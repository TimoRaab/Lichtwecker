

#include "timeInformation.h"
#include "sdCard.h"

//const char* ssid     = "SetYourSSID";
//const char* password = "SetYourPassword";
const char* ntpServer = "pool.ntp.org";
long  gmtOffset_sec = 3600;
int   daylightOffset_sec = 3600;

#include <Esp.h>

byte setup_TimeInformation() {
    #define RTC_UTC_TEST 1510592825 // 1510592825 = Monday 13 November 2017 17:07:05 UTC

    time_t rtc = RTC_UTC_TEST;
    timeval tv = { rtc, 0 };
    settimeofday(&tv, nullptr);
    return setTimeInformation();
}

byte setTimeInformation() {
    if (WiFi.status() == WL_CONNECTED) {
        struct tm timeinfo = getCurrentTimeStruct();
        struct tm timeinfo2 = getCurrentTimeStruct();
        int tempCounter = 0;
        delay(500);
        while(timeinfo.tm_year == timeinfo2.tm_year && tempCounter < 10) {
            configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
            delay(1000);
            timeinfo2 = getCurrentTimeStruct();
            Serial.print("Still Searching, TempCounter: ");
            Serial.println(tempCounter);
            tempCounter++;
        }
        //WiFi.disconnect(true);
        //WiFi.mode(WIFI_OFF);
        if (tempCounter >= 10) return 10;
        return 1;
    }
    return 100;
    /*if (!sdCard_open()) return 0; 
    writeFile(SD, "/hello.txt", "Hello ");
    File root = SD.open("/WLAN");
    int tempCounter = 0;
    LinkedList<String> fName = findFilesInDirectory(root);
    root.close();
    while(fName.size() == 0 && tempCounter < 5) {
        root = SD.open("/WLAN");
        delay(100);
        tempCounter++;
        fName = findFilesInDirectory(root);
        root.close();
    }
    if (fName.size() == 0) {
        Serial.println("No WLAN files found!");
        return 10;
    }
    String SSID = "";
    String pw = "";
    WiFi.mode(WIFI_STA);
    for (int i=0; i < fName.size(), i++) {
        getWLANInformation("/WLAN/" + fName.get(i), SSID, pw);
        WiFi.begin(ssid, password);
        long tempMillis = millis();
        while (WiFi.status() != WL_CONNECTED && millis()-tempMillis < 10000) {
            delay(500);
        }

        if (WiFi.status() == WL_CONNECTED) {
            configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
            WiFi.disconnect(true);
            WiFi.mode(WIFI_OFF);
            return 1;
        }
    }
    WiFi.mode(WIFI_OFF);
    Serial.println("WIFI Access not possible!");
    return 100;*/
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

void setGMTOffset(long sec) {
    gmtOffset_sec = sec;
}

long getGMTOffset() {
    return gmtOffset_sec;
}

void setDaylightOffset(int sec) {
    daylightOffset_sec = sec;
}

int getDaylightOffset() {
    return daylightOffset_sec;
}
//EOF