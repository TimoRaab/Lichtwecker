//#include <Arduino.h>
//#include <TFT_eSPI.h>
//#include <LCDMenuLib2.h>  
#include "menu.h"

TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h

uint32_t targetTime = 0;       // for next 1 second timeout

byte omm = 99;
bool initial = 1;
byte xcolon = 0;
unsigned int colour = 0;

static uint8_t conv2d(const char* p) {
  uint8_t v = 0;
  if ('0' <= *p && *p <= '9')
    v = *p - '0';
  return 10 * v + *++p - '0';
}

uint8_t hh=conv2d(__TIME__), mm=conv2d(__TIME__+3), ss=conv2d(__TIME__+6);  // Get H, M, S from compile time


//Button bRight = Button(buttonRight);
//Button bLeft = Button(buttonLeft);
Button bUp = Button(15);
Button bDown = Button(4);
Button bOK = Button(12);
Button bAbort = Button(14);

extern bool playAudio;

#define _LCDML_ADAFRUIT_TEXT_COLOR       0xFFFF
  #define _LCDML_ADAFRUIT_BACKGROUND_COLOR 0x0000 
  
  #define _LCDML_ADAFRUIT_FONT_SIZE   3   
  #define _LCDML_ADAFRUIT_FONT_W      (6*_LCDML_ADAFRUIT_FONT_SIZE)             // font width 
  #define _LCDML_ADAFRUIT_FONT_H      (8*_LCDML_ADAFRUIT_FONT_SIZE)             // font heigt 
  
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
  LCDML_add         (0  , LCDML_0         , 1  , "Information"      , NULL);       // this menu function can be found on "LCDML_display_menuFunction" tab
  LCDML_add         (1  , LCDML_0         , 2  , "Time info"        , NULL);        // this menu function can be found on "LCDML_display_menuFunction" tab
  LCDML_add         (2  , LCDML_0         , 3  , "Program"          , NULL);                    // NULL = no menu function
  LCDML_add         (3  , LCDML_0_3       , 1  , "Program 1"        , NULL);                    // NULL = no menu function
  LCDML_add         (4  , LCDML_0_3_1     , 1  , "P1 dummy"         , NULL);                    // NULL = no menu function
  LCDML_add         (5  , LCDML_0_3_1     , 2  , "P1 Settings"      , NULL);                    // NULL = no menu function
  LCDML_add         (6  , LCDML_0_3_1_2   , 1  , "Warm"             , NULL);                    // NULL = no menu function
  LCDML_add         (7  , LCDML_0_3_1_2   , 2  , "Cold"             , NULL);                    // NULL = no menu function
  LCDML_add         (8  , LCDML_0         , 4  , "ProgramA"          , NULL);                    // NULL = no menu function
  LCDML_add         (9  , LCDML_0_3       , 2  , "ProgramA 1"        , NULL);                    // NULL = no menu function
  LCDML_add         (10 , LCDML_0_3       , 3  , "P1 dummyA"         , NULL);                    // NULL = no menu function
  LCDML_add         (11 , LCDML_0_3       , 4  , "P1 SettingsA"      , NULL);                    // NULL = no menu function
  LCDML_add         (12 , LCDML_0_3       , 5  , "WarmA"             , NULL);                    // NULL = no menu function
  LCDML_add         (13 , LCDML_0_3       , 6  , "ColdA"             , NULL);                    // NULL = no menu function
  LCDML_add         (14 , LCDML_0         , 5  , "StartAudio"        , startAudio);

  // ***TIP*** Try to update _LCDML_DISP_cnt when you add a menu element.

  // menu element count - last element id
  // this value must be the same as the last menu element
  #define _LCDML_DISP_cnt    14

  // create menu
  LCDML_createMenu(_LCDML_DISP_cnt);

extern Audio audio;

void startAudio(byte param) {
  if (LCDML.FUNC_setup()) {
    Serial.println("Start Audio");
    tft.fillScreen(_LCDML_ADAFRUIT_BACKGROUND_COLOR);
    audio.
    audio.connecttoFS(SD,"/MYMUSIC.mp3");
    playAudio = true;
  }

  if(LCDML.FUNC_loop()) {
    playAudio = true;
    if (LCDML.BT_checkAny()) {
      LCDML.FUNC_goBackToMenu();
    }
  }

  if (LCDML.FUNC_close()) {
    playAudio = false;
    LCDML.MENU_getCurrentObj();
  }
}

void menuSetup() {
        /* INIT DISPLAY */
    tft.init();
    tft.setRotation(1);


    // clear lcd
    tft.fillScreen(_LCDML_ADAFRUIT_BACKGROUND_COLOR);
    
    // set text color / Textfarbe setzen
    tft.setTextColor(_LCDML_ADAFRUIT_TEXT_COLOR);  
    // set text size / Textgroesse setzen
    tft.setTextSize(_LCDML_ADAFRUIT_FONT_SIZE);
    tft.setCursor(0, _LCDML_ADAFRUIT_FONT_H * (3));
    //tft.println(_LCDML_VERSION);

    /* INIT LCDML */
    // LCDMenuLib Setup
    LCDML_setup(_LCDML_DISP_cnt);

    // Enable Menu Rollover
    LCDML.MENU_enRollover();

    // Enable Screensaver (screensaver menu function, time to activate in ms)
    //LCDML.SCREEN_disable();

    // Some needful methods

    // You can jump to a menu function from anywhere with
    //LCDML.OTHER_jumpToFunc(mFunc_p2); // the parameter is the function name
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
  //if (bLeft.isPressed(false)) return 13;
  //if (bRight.isPressed(false)) return 14;
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
    case 13: LCDML.BT_left(); break;
    case 14: LCDML.BT_right(); break;
    default: break;
  }
  if (Serial.available()) {
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
  LCDML.loop();
}