#include <Arduino.h>
#include <TFT_eSPI.h>
#include <LCDMenuLib2.h>  
#include <Button.h>
#include "Audio.h"


extern LCDMenuLib2 LCDML;
//extern Audio audio;

const byte okButtonPin = 23;
const byte abortButtonPin = 21;
const byte upButtonPin = 2;
const byte downButtonPin = 4;

void startAudio(byte param);
void menuSetup();
void menuStart();
void lcdml_menu_display();
void lcdml_menu_clear();
void lcdml_menu_control();

