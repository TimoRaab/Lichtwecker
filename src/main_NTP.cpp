//v01
//Autor: Timo Raab
//
// include display to show time

#include <Arduino.h>
//#include <SPI.h>
//#include <TFT_eSPI.h>
//#include "FreeSans36pt7b.h"
#include "timeInformation.h"
#include "menuLightAlarm.h"

/*
#define FSS9 &FreeSans9pt7b
#define FSS12 &FreeSans12pt7b
#define FSS18 &FreeSans18pt7b
#define FSS24 &FreeSans24pt7b
#define FSS36 &FreeSans36pt7b


#define _LCDML_ADAFRUIT_TEXT_COLOR       0x9999
#define _LCDML_ADAFRUIT_BACKGROUND_COLOR 0x0000 

#define _LCDML_ADAFRUIT_FONT_SIZE   1  
#define _LCDML_ADAFRUIT_FONT_W      (6*_LCDML_ADAFRUIT_FONT_SIZE)             // font width 
#define _LCDML_ADAFRUIT_FONT_H      (8*_LCDML_ADAFRUIT_FONT_SIZE)             // font heigt 

// settings for u8g lib and lcd
#define _LCDML_ADAFRUIT_lcd_w       240            // lcd width
#define _LCDML_ADAFRUIT_lcd_h       135             // lcd height
 

TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h
int backlightPin = 4;



bool playAudio = false;

String tempDateString = "";
String tempTimeString = "";*/


void setup() {
    Serial.begin(115200);
    setup_TimeInformation();
    setup_menu();
    /*
    pinMode(backlightPin, OUTPUT);

    tft.init();
    tft.setRotation(1);
    tft.fillScreen(_LCDML_ADAFRUIT_BACKGROUND_COLOR);
    tft.setTextColor(tft.color565(200,200,200));  

    ledcSetup(0, 5000, 8);
    ledcAttachPin(backlightPin, 0);
    ledcWrite(0, 255);*/

    Serial.println("Startup Finished");
}












 void loop() {
    menuStart();
    /*
    if (Serial.available()) {
        String tempString = Serial.readStringUntil('\n');
        int tempBacklight = tempString.toInt();
        Serial.println(tempBacklight);
        ledcWrite(0, tempBacklight);
    }

    if (!getDate().equals(tempDateString)) {
        tempDateString = getDate();
        tft.fillRect(20,6,200,24, tft.color565(0,0,0));
        tft.setTextSize(1);
        tft.setFreeFont(FSS12);
        tft.setCursor(20, 25);
        tft.println(getDate());
    }

    if (!getTime().equals(tempTimeString)) {
        tempTimeString = getTime();
        tft.fillRect(35,50,180,55, tft.color565(0,0,0));
        tft.setCursor(35, 100);
        //tft.setTextSize(2);
        tft.setFreeFont(FSS36);
        tft.println(getTime());
        tft.setTextSize(1);
    }
 
    vTaskDelay(pdMS_TO_TICKS(500));*/
}
