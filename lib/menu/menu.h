#include <Arduino.h>
#include <TFT_eSPI.h>
#include <LCDMenuLib2.h>  

extern LCDMenuLib2 LCDML;

const byte okButtonPin = 33;
const byte abortButtonPin = 34;
const byte upButtonPin = 35;
const byte downButtonPin = 36;

void menuSetup();
void lcdml_menu_display();
void lcdml_menu_clear();
void lcdml_menu_control();


// I just wanted to test something