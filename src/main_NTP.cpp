//v01
//Autor: Timo Raab
//
// include display to show time

#include <Arduino.h>
#include "timeInformation.h"
#include "menuLightAlarm.h"
#include "sdCard.h"
#include "musicPlayer.h"
#include "ledLight.h"
#include "wifiHandling.h"
#include <LinkedList.h>
#include <WiFiManager.h>



void setup() {
    Serial.begin(115200);
    setup_WIFI();
    setup_TimeInformation();
    //setup_SDCard();
    //setup_musicPlayer();
    //setup_menu();
    //setup_LED();
    //delay(500);
    Serial.println("Startup Finished");
    for (int i=0; i<3; i++) {
        Serial.println(getTime());
            delay(1000);
    }

    WIFIconnect();
    Serial.println(setTimeInformation());
    WIFIdisconnect();
    Serial.println("Startup Finished");
    for (int i=0; i<3; i++) {
        Serial.println(getTime());
            delay(1000);
    }

}

 void loop() {
    Serial.println(getTime());
    delay(5000);
    //menuStart();
    //updateLED();
}
