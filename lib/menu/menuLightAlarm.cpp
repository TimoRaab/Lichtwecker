//#include <Arduino.h>
//#include <TFT_eSPI.h>
//#include <LCDMenuLib2.h>  
#include "menuLightAlarm.h"

#include <SPI.h>
#include <Audio.h>
#include <SD.h>
#include <FS.h>
#include <FastLED.h>


#include "FreeSans36pt7b.h"
#define FSS9 &FreeSans9pt7b
#define FSS12 &FreeSans12pt7b
#define FSS18 &FreeSans18pt7b
#define FSS24 &FreeSans24pt7b
#define FSS36 &FreeSans36pt7b


TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h

/*
//Button bRight = Button(buttonRight);
//Button bLeft = Button(buttonLeft);
Button bUp = Button(35, true, false, 50, NULL, true);
Button bDown = Button(34, true, false, 50, NULL, true);
Button bOK = Button(39, true, false, 50, NULL, true);
Button bAbort = Button(36, true, false, 50, NULL, true);
*/

ButtonDebounce bUp = ButtonDebounce(34, true);
ButtonDebounce bDown = ButtonDebounce(35, true);
ButtonDebounce bOK = ButtonDebounce(39, true);
ButtonDebounce bAbort = ButtonDebounce(36, true);

byte backlightPin = 4;
long screenSaverMillis = 0;
boolean darkScreen = false;
String tempDateString = "";
String tempTimeString = "";

Audio audio;


#define LED_PIN_8     32
#define NUM_LEDS_8    10 
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
CRGB leds_8[NUM_LEDS_8];
uint8_t brightness = 90;
#define UPDATES_PER_SECOND 2
CRGBPalette16 currentPalette;
TBlendType    currentBlending;

// microSD Card Reader connections
//define SD_CS         5
//#define SPI_MOSI      19 
//#define SPI_MISO      23
//#define SPI_SCK       18
 
#define SD_CS         15
#define SPI_MOSI      12 
#define SPI_MISO      13
#define SPI_SCK       14


// I2S Connections
#define I2S_DOUT      25
#define I2S_BCLK      26
#define I2S_LRC       27

#define _LCDML_ADAFRUIT_TEXT_COLOR       0xFFFF
  #define _LCDML_ADAFRUIT_BACKGROUND_COLOR 0x0000 
  
  #define _LCDML_ADAFRUIT_FONT_SIZE   1  
  #define _LCDML_ADAFRUIT_FONT_W      (10*_LCDML_ADAFRUIT_FONT_SIZE)             // font width 
  #define _LCDML_ADAFRUIT_FONT_H      (12*_LCDML_ADAFRUIT_FONT_SIZE)             // font heigt 
  
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




// *********************************************************************
// Objects
// *********************************************************************
  LCDMenuLib2_menu LCDML_0 (255, 0, 0, NULL, NULL); // root menu element (do not change)
  LCDMenuLib2 LCDML(LCDML_0, _LCDML_DISP_rows, _LCDML_DISP_cols, lcdml_menu_display, lcdml_menu_clear, lcdml_menu_control);

boolean COND_hide()  // hide a menu element
// ********************************************************************* 
{ 
  return false;  // hidden
} 

// *********************************************************************
// LCDML MENU/DISP
// *********************************************************************
  // LCDML_0        => layer 0
  // LCDML_0_X      => layer 1
  // LCDML_0_X_X    => layer 2
  // LCDML_0_X_X_X  => layer 3
  // LCDML_0_...      => layer ...

  // For beginners
  // LCDML_add(id, prev_layer, new_num, lang_char_array, callback_function)
  LCDML_add         (0  , LCDML_0         , 1  , "Set Time"      , NULL);       
  LCDML_add         (1  , LCDML_0         , 2  , "Set Sleepcasts"   , NULL);
  LCDML_add         (2  , LCDML_0         , 3  , "Set Alarm"        , NULL);        
  LCDML_add         (3  , LCDML_0         , 4  , "Set Wakeup Tune"  , NULL);
  LCDML_add         (4  , LCDML_0         , 5  , "Set Wakeup Music" , NULL);   
  LCDML_add         (5  , LCDML_0_1       , 1, "A", NULL);
  LCDML_add         (6  , LCDML_0_1       , 2, "B", NULL);
  LCDML_add         (7  , LCDML_0_1       , 3, "C", NULL);
  LCDML_add         (8  , LCDML_0_1       , 4, "D", NULL);
  LCDML_add         (9  , LCDML_0_2       , 1, "A", NULL);
  LCDML_add         (10 , LCDML_0_2       , 2, "Play Music", playMusic);

  LCDML_addAdvanced (11 , LCDML_0         , 6  , COND_hide,  "screensaver"        , mFunc_screensaver,        0,   _LCDML_TYPE_default);  

  // ***TIP*** Try to update _LCDML_DISP_cnt when you add a menu element.

  // menu element count - last element id
  // this value must be the same as the last menu element
  #define _LCDML_DISP_cnt    11

  // create menu
  LCDML_createMenu(_LCDML_DISP_cnt);

SPIClass * hspi = NULL;

void setup_menu() {


    
    // Initialize SPI bus for microSD Card
    //SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);
    pinMode(SD_CS, OUTPUT);
    digitalWrite(SD_CS, HIGH);


    hspi = new SPIClass(HSPI);
    hspi->begin(SPI_SCK, SPI_MISO, SPI_MOSI, SD_CS);
    //sdSPI.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);
    // Start microSD Card
    Serial.println("Here");
    if(!SD.begin(SD_CS, *hspi))
    {
      Serial.println("Error accessing microSD card!");
      while(true); 
    }
    Serial.println("TADA");
    
    // Setup I2S 
    audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
    
    // Set Volume
    audio.setVolume(12);
    
    // Open music file
    audio.connecttoFS(SD,"/MYMUSIC.mp3");


    tft.init();
    tft.setRotation(1);
    tft.fillScreen(_LCDML_ADAFRUIT_BACKGROUND_COLOR);
    tft.setTextColor(tft.color565(200,200,200));  

    pinMode(backlightPin, OUTPUT);
    ledcSetup(0, 1000, 8);
    ledcAttachPin(backlightPin, 0);
    ledcWrite(0, 255);
  
    LCDML_setup(_LCDML_DISP_cnt);
    LCDML.MENU_enRollover();
    LCDML.SCREEN_enable(mFunc_screensaver, 10000); // set to 10 seconds

    FastLED.addLeds<LED_TYPE, LED_PIN_8, COLOR_ORDER>(leds_8, NUM_LEDS_8).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(brightness);

    
    currentPalette = RainbowColors_p;
    currentBlending = LINEARBLEND;
}


  # define _LCDML_CONTROL_serial_enter           'e'
  # define _LCDML_CONTROL_serial_up              'w'
  # define _LCDML_CONTROL_serial_down            's'
  # define _LCDML_CONTROL_serial_left            'a'
  # define _LCDML_CONTROL_serial_right           'd'
  # define _LCDML_CONTROL_serial_quit            'q'

byte checkButtons() {
  if (bOK.isPressed(false)) return 0;
  if (bAbort.isPressed(false)) return 1;
  if (bUp.isPressed(false))  return 11;
  if (bDown.isPressed(false)) return 12;
  return 255;
}

void lcdml_menu_control(void)
{
  // If something must init, put in in the setup condition
  if(LCDML.BT_setup()) {
  }
  if(LCDML.CE_setup()) {
  }
  
  uint8_t buttonValue = checkButtons();
  switch(buttonValue) {
    case 0:  LCDML.BT_enter(); break;
    case 1:  LCDML.BT_quit(); break;
    case 11: LCDML.BT_up(); break;
    case 12: LCDML.BT_down(); break;
    default: break;
  }
  if (Serial.available()) {
    Serial.println("Serial?");
    switch (Serial.read()) {
      case _LCDML_CONTROL_serial_enter:  LCDML.BT_enter(); break;
      case _LCDML_CONTROL_serial_up:     LCDML.BT_up();    break;
      case _LCDML_CONTROL_serial_down:   LCDML.BT_down();  break;
      case _LCDML_CONTROL_serial_left:   LCDML.BT_left();  break;
      case _LCDML_CONTROL_serial_right:  LCDML.BT_right(); break;
      case _LCDML_CONTROL_serial_quit:   LCDML.BT_quit();  break;
      default: break;
    }
  }
}




void lcdml_menu_display()
/* ******************************************************************** */
{
  
  // clear lcd
  tft.fillScreen(_LCDML_ADAFRUIT_BACKGROUND_COLOR);
  // set text color / Textfarbe setzen
  tft.setTextColor(_LCDML_ADAFRUIT_TEXT_COLOR);  
  // set text size / Textgroesse setzen
  tft.setTextSize(_LCDML_ADAFRUIT_FONT_SIZE);
  tft.setTextFont(1);
  
  // declaration of some variables
  // ***************
  // content variable
  char content_text[_LCDML_DISP_cols];  // save the content text of every menu element
  // menu element object
  LCDMenuLib2_menu *tmp;
  // some limit values
  uint8_t i = LCDML.MENU_getScroll();
  uint8_t maxi = _LCDML_DISP_rows + i;
  uint8_t n = 0;

   // init vars
  uint8_t n_max             = (LCDML.MENU_getChilds() >= _LCDML_DISP_rows) ? _LCDML_DISP_rows : (LCDML.MENU_getChilds());

  uint8_t scrollbar_min     = 0;
  uint8_t scrollbar_max     = LCDML.MENU_getChilds();
  uint8_t scrollbar_cur_pos = LCDML.MENU_getCursorPosAbs();
  uint8_t scroll_pos        = ((1.*n_max * _LCDML_DISP_rows) / (scrollbar_max - 1) * scrollbar_cur_pos);

  n = 0;
  i = LCDML.MENU_getScroll();
  // update content
  // ***************

    // clear menu
    // ***************

  // check if this element has children
  if ((tmp = LCDML.MENU_getDisplayedObj()) != NULL)
  {
    // loop to display lines
    do
    {
      // check if a menu element has a condition and if the condition be true
      if (tmp->checkCondition())
      {
        // check the type off a menu element
        if(tmp->checkType_menu() == true)
        {
          // display normal content
          LCDML_getContent(content_text, tmp->getID());
          tft.setCursor(10, _LCDML_ADAFRUIT_FONT_H * (n));
          tft.println(content_text);
        }
        else
        {
          if(tmp->checkType_dynParam()) {
            tmp->callback(n);
          }
        }
        // increment some values
        i++;
        n++;
      }
    // try to go to the next sibling and check the number of displayed rows
    } while (((tmp = tmp->getSibling(1)) != NULL) && (i < maxi));
  }

  // set cursor
  tft.setCursor(0, _LCDML_ADAFRUIT_FONT_H * (LCDML.MENU_getCursorPos()));
  tft.println("X");

  
}

/* ******************************************************************** */
void lcdml_menu_clear()
/* ******************************************************************** */
{
}

void menuStart() {
  bUp.updateButton();
  bDown.updateButton();
  bOK.updateButton();
  bAbort.updateButton();
  LCDML.loop();
  static uint8_t startIndex = 0;
startIndex = startIndex + 1; /* motion speed */

  FillLEDsFromPaletteColors( startIndex);
    
    FastLED.show();
}


void mFunc_screensaver(uint8_t param) {
  if (LCDML.FUNC_setup()) {
    LCDML_UNUSED(param);
    tft.fillScreen(_LCDML_ADAFRUIT_BACKGROUND_COLOR);
    darkScreen = false;
    ledcWrite(0, 200);
        tempDateString = getDate();
        tft.fillRect(20,6,200,24, tft.color565(0,0,0));
        tft.setTextSize(1);
        tft.setFreeFont(FSS12);
        tft.setCursor(20, 25);
        tft.println(getDate());

        tempTimeString = getTime();
        tft.fillRect(35,50,180,55, tft.color565(0,0,0));
        tft.setCursor(35, 100);
        //tft.setTextSize(2);
        tft.setFreeFont(FSS36);
        tft.println(getTime());
        tft.setTextSize(1);

    screenSaverMillis = millis();

    LCDML.FUNC_setLoopInterval(200);  // starts a trigger event for the loop function every 100 milliseconds
  }

  if(LCDML.FUNC_loop()) {
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
        tft.setFreeFont(FSS36);
        tft.println(getTime());
        tft.setTextSize(1);
    }
    if (millis()-screenSaverMillis > 10000) {
      darkScreen = true;
      ledcWrite(0, 30);
    }

    if (LCDML.BT_checkAny() || LCDML.BT_checkQuit()) {
      if (darkScreen) {
        darkScreen = false;
        ledcWrite(0, 200);
        screenSaverMillis = millis();
      } else {
        LCDML.FUNC_goBackToMenu();  // leave this function
      }
    }
  }

  /*if(LCDML.FUNC_close())          // ****** STABLE END *********
  {
    // The screensaver go to the root menu
    Serial.println("Close");
    LCDML.MENU_goRoot();
  }*/
}

void playMusic(uint8_t param) {
  if (LCDML.FUNC_setup()) {
    LCDML_UNUSED(param);
    tft.fillScreen(_LCDML_ADAFRUIT_BACKGROUND_COLOR);
    LCDML.FUNC_disableScreensaver();

        tempTimeString = getTime();
        tft.fillRect(35,50,180,55, tft.color565(0,0,0));
        tft.setCursor(35, 100);
        //tft.setTextSize(2);
        tft.setFreeFont(FSS36);
        tft.println("PLAY");
        tft.setTextSize(1);
        audio.connecttoFS(SD,"/MYMUSIC.mp3");

    //LCDML.FUNC_setLoopInterval(100);  
  }

  if(LCDML.FUNC_loop()) {
    do {
      audio.loop();
        bUp.updateButton();
        bDown.updateButton();
        bOK.updateButton();
        bAbort.updateButton();
        //LCDML.SCREEN_resetTimer();
    } while (checkButtons() == 255);
      LCDML.FUNC_goBackToMenu();
      audio.pauseResume();
      //audio.stopSong();
  }

  if(LCDML.FUNC_close())          // ****** STABLE END *********
  {
    // The screensaver go to the root menu
    Serial.println("Close");
    LCDML.MENU_goRoot();
  }
}

void FillLEDsFromPaletteColors( uint8_t colorIndex) {
    for (int i=0; i < NUM_LEDS_8; i++) {
        leds_8[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        colorIndex +=10;
    }
}