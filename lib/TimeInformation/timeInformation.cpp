

#include "timeInformation.h"
#include "sdCard.h"
#include "wifiHandling.h"

extern boolean WIFIavailable;

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

//0 set time
//10 wifi connected, but not possible to reach server
//100 wifi not connnected
//200 wifi not available
byte setTimeInformation() {
    if (!WIFIavailable) return 200;
    WIFIconnect();
    if (WiFi.status() == WL_CONNECTED) {
        struct tm timeinfo = getCurrentTimeStruct();
        struct tm timeinfo2 = getCurrentTimeStruct();
        int tempCounter = 0;
        delay(500);
        configTime(gmtOffset_sec, daylightOffset_sec, "de.pool.ntp.org", "pool.ntp.org", "time.nist.gov");
        long tempMillis = millis();
        while(timeinfo.tm_year == timeinfo2.tm_year && millis()-tempMillis < 20000) {          
            timeinfo2 = getCurrentTimeStruct();
            Serial.print("Still Searching, TempCounter: ");
            Serial.println(tempCounter);
            tempCounter++;
            delay(100);
        }
        WIFIdisconnect();
        if (tempCounter >= 10) return 10;
        return 0;
    }
    WIFIdisconnect();
    return 100;
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