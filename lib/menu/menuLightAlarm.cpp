
#include "menuLightAlarm.h"

#include <Audio.h>
#include <SPI.h>
#include <TaskScheduler.h>

#include "musicPlayer.h"
#include "ledLight.h"
#include "timeInformation.h"
#include "setTime/setTimeFunctions.h"

#include "FreeSans36pt7b.h"
#define FSS9 &FreeSans9pt7b
#define FSS12 &FreeSans12pt7b
#define FSS18 &FreeSans18pt7b
#define FSS24 &FreeSans24pt7b
#define FSS36 &FreeSans36pt7b


TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h


ButtonDebounce bUp = ButtonDebounce(34, true);
ButtonDebounce bDown = ButtonDebounce(35, true);
ButtonDebounce bOK = ButtonDebounce(39, true);
ButtonDebounce bAbort = ButtonDebounce(36, true);

Task updateButton(10, TASK_FOREVER, &updateButtonHistory);
Scheduler buttonRunner;

byte backlightPin = 4;
long screenSaverMillis = 0;
boolean darkScreen = false;
String tempDateString = "";
String tempTimeString = "";

unsigned long timeMeasure = 0;




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
LCDML_addAdvanced (0 , LCDML_0         , 200  , COND_hide,  "screensaver"        , mFunc_screensaver,        0,   _LCDML_TYPE_default);  

LCDML_add         (1  , LCDML_0         , 1  , "Set Alarm"      , NULL);       
LCDML_add         (2  , LCDML_0         , 2  , "Set Sleepcasts"   , NULL);
LCDML_add         (3  , LCDML_0         , 3  , "Set Alarm"        , NULL);        
LCDML_add         (4  , LCDML_0         , 4  , "Set Wakeup Tune"  , NULL);
LCDML_add         (5  , LCDML_0         , 5  , "Set Wakeup Music" , NULL);  

LCDML_add         (6  , LCDML_0         , 6  , "Time Settings"    , NULL);
  LCDML_add         (7  , LCDML_0_6       , 1  , "NTP Settings"     , NULL);
    LCDML_add         (8  , LCDML_0_6_1     , 1  , "Activate NPT"     , NULL); //todo
    LCDML_add         (9  , LCDML_0_6_1     , 2  , "NTP Intervall"    , NULL); //TODO
  LCDML_add         (10 , LCDML_0_6       , 2  , "Set Time NTP now" , NULL); //TODO
  LCDML_add         (11 , LCDML_0_6       , 3  , "Set Time Manual"  , menu_setTimeManual);        
    LCDML_addAdvanced (12 , LCDML_0_6_3     , 1  , NULL, "", menu_setHour, 0, _LCDML_TYPE_dynParam);
    LCDML_addAdvanced (13 , LCDML_0_6_3     , 2  , NULL, "", menu_setMinute, 0, _LCDML_TYPE_dynParam);
    LCDML_addAdvanced (14 , LCDML_0_6_3     , 4  , NULL, "", menu_setMonth, 0, _LCDML_TYPE_dynParam);
    LCDML_addAdvanced (15 , LCDML_0_6_3     , 3  , NULL, "", menu_setDay, 0, _LCDML_TYPE_dynParam);
    LCDML_addAdvanced (16 , LCDML_0_6_3     , 5  , NULL, "", menu_setYear, 0, _LCDML_TYPE_dynParam);
    LCDML_addAdvanced (17 , LCDML_0_6_3     , 6  , NULL, "Reset Seconds", menu_resetSeconds, 0, _LCDML_TYPE_dynParam);

LCDML_add         (18  , LCDML_0_1       , 1, "A", NULL);
LCDML_add         (19  , LCDML_0_1       , 2, "B", NULL);
LCDML_add         (20  , LCDML_0_1       , 3, "C", NULL);
LCDML_add         (21 , LCDML_0_1       , 4, "D", NULL);
LCDML_add         (22 , LCDML_0_2       , 1, "A", NULL);
LCDML_add         (23 , LCDML_0_2       , 2, "Play Music", playMusic);

  // ***TIP*** Try to update _LCDML_DISP_cnt when you add a menu element.

  // menu element count - last element id
  // this value must be the same as the last menu element
  #define _LCDML_DISP_cnt    23

  // create menu
  LCDML_createMenu(_LCDML_DISP_cnt);


void setup_menu() {
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

    buttonRunner.init();
    buttonRunner.addTask(updateButton);
    updateButton.enable();
    buttonRunner.startNow();

}

void updateButtonHistory() {
  bOK.updateButton();
  bAbort.updateButton();
  bUp.updateButton();
  bDown.updateButton();
}




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
  buttonRunner.execute();
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
  tft.setTextFont(2);
  
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
        setCurrentMusic("/WAKEUPTUNE/MyMusic.mp3");
        changeBrightness(90);

        timeMeasure = millis();
    //LCDML.FUNC_setLoopInterval(100);  
  }

  if(LCDML.FUNC_loop()) {
    while (true) {
      playMusic();
      updateLED();
      lcdml_menu_control();
        //LCDML.SCREEN_resetTimer();
      if (LCDML.BT_checkUp()) {
        changeVolume(1);
        LCDML.BT_resetAll();
      }
      if (LCDML.BT_checkDown()) {
        changeVolume(-1);
        LCDML.BT_resetAll();
      }
      if (LCDML.BT_checkEnter() || LCDML.BT_checkQuit()) {
        stopMusic();
        LCDML.FUNC_goBackToMenu();
        break;
      }
      if (isSongFinished()) {
        if (isSongFinished() == 1) {
          stopMusic();
          LCDML.FUNC_goBackToMenu();
          break;
        } 
      }
    }
  }

  if(LCDML.FUNC_close())          // ****** STABLE END *********
  {
    // The screensaver go to the root menu
    LCDML.MENU_goRoot();
  }
}



//EOF