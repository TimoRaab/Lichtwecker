#ifndef menuLightAlarm_h
#define menuLightAlarm_h

#include <Arduino.h>
#include <TFT_eSPI.h>
#include <LCDMenuLib2.h>  
#include <ButtonDebounce.h>
#include "Audio.h"
#include "menu.h"
#include "timeInformation.h"


extern LCDMenuLib2 LCDML;
//extern Audio audio;

const byte okButtonPin = 23;
const byte abortButtonPin = 21;
const byte upButtonPin = 2;
const byte downButtonPin = 4;

void startAudio(byte param);
void setup_menu();
void menuStart();
void lcdml_menu_display();
void lcdml_menu_clear();
void lcdml_menu_control();

void mFunc_screensaver(uint8_t param);
void playMusic(uint8_t param);
void FillLEDsFromPaletteColors(uint8_t colorIndex);

#endif
//EOF