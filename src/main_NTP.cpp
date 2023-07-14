//v01
//Autor: Timo Raab
//
// include display to show time

#include <Arduino.h>
#include "timeInformation.h"
#include "menuLightAlarm.h"
#include "sdCard.h"
#include "musicPlayer.h"
#include <LinkedList.h>



void setup() {
    Serial.begin(115200);
    setup_SDCard();
    setup_musicPlayer();
    setup_TimeInformation();
    setup_menu();
    delay(500);
    Serial.println("Startup Finished");

    File root = SD.open("/WLAN");
    //printDirectory(root, 0);
    int tempCounter = 0;
    LinkedList<String> fName = findFilesInDirectory(root);
    while(fName.size() == 0 && tempCounter < 10) {
        delay(100);
        Serial.println("Error File Size!, Try: " + tempCounter);
        tempCounter++;
        fName = findFilesInDirectory(root);
    }

    Serial.println("GetData from LinkedList");
    Serial.println("Finished");
    String SSID = "";
    String pw = "";
    getWLANInformation("/WLAN/" + fName.get(0), SSID, pw);

    Serial.println("Outside");
    Serial.println(SSID);
    Serial.println(pw);
}


 void loop() {
    menuStart();
}
