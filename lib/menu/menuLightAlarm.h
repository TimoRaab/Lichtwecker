#ifndef menuLightAlarm_h
#define menuLightAlarm_h

#include <Arduino.h>
#include <TFT_eSPI.h>
#include <LCDMenuLib2.h>  
#include <ButtonDebounce.h>
#include "timeInformation.h"


extern LCDMenuLib2 LCDML;

const byte okButtonPin = 23;
const byte abortButtonPin = 21;
const byte upButtonPin = 2;
const byte downButtonPin = 4;

# define _LCDML_CONTROL_serial_enter           'e'
# define _LCDML_CONTROL_serial_up              'w'
# define _LCDML_CONTROL_serial_down            's'
# define _LCDML_CONTROL_serial_left            'a'
# define _LCDML_CONTROL_serial_right           'd'
# define _LCDML_CONTROL_serial_quit            'q'

void startAudio(byte param);
void setup_menu();
void menuStart();
void lcdml_menu_display();
void lcdml_menu_clear();
void lcdml_menu_control();

void mFunc_screensaver(uint8_t param);
void playMusic(uint8_t param);
void FillLEDsFromPaletteColors(uint8_t colorIndex);

void menu_setHour(uint8_t line);
void menu_setMinute(uint8_t line);
void menu_setMonth(uint8_t line);
void menu_setDay(uint8_t line);

void updateButtonHistory();

#endif
//EOF