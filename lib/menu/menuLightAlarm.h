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

#define _LCDML_ADAFRUIT_TEXT_COLOR       0xFFFF
#define _LCDML_ADAFRUIT_BACKGROUND_COLOR 0x0000 
  
#define _LCDML_ADAFRUIT_FONT_SIZE   1  
#define _LCDML_ADAFRUIT_FONT_W      (10*_LCDML_ADAFRUIT_FONT_SIZE)             // font width 
#define _LCDML_ADAFRUIT_FONT_H      (20*_LCDML_ADAFRUIT_FONT_SIZE)             // font heigt 
  
// settings for u8g lib and lcd
#define _LCDML_ADAFRUIT_lcd_w       240            // lcd width
#define _LCDML_ADAFRUIT_lcd_h       135             // lcd height





// nothing change here
#define _LCDML_ADAFRUIT_cols_max    (_LCDML_ADAFRUIT_lcd_w/_LCDML_ADAFRUIT_FONT_W)  
#define _LCDML_ADAFRUIT_rows_max    (_LCDML_ADAFRUIT_lcd_h/_LCDML_ADAFRUIT_FONT_H) 

// rows and cols 
// when you use more rows or cols as allowed change in LCDMenuLib.h the define "_LCDML_DISP_cfg_max_rows" and "_LCDML_DISP_cfg_max_string_length"
// the program needs more ram with this changes
#define _LCDML_ADAFRUIT_cols        20                   // max cols
#define _LCDML_ADAFRUIT_rows        _LCDML_ADAFRUIT_rows_max  // max rows 


// scrollbar width
#define _LCDML_ADAFRUIT_scrollbar_w 6  // scrollbar width  


// old defines with new content
#define _LCDML_DISP_cols      _LCDML_ADAFRUIT_cols
#define _LCDML_DISP_rows      _LCDML_ADAFRUIT_rows 

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