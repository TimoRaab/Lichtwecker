#include "setTimeFunctions.h"
#include "timeInformation.h"
#include "menuLightAlarm.h"

extern LCDMenuLib2 LCDML;
extern TFT_eSPI tft;

uint16_t menu_tempTimeConstant = 0;
struct tm menu_tempTimeStruct;
boolean timeChangeFlag = false;

const char* const timeIdentifierLookup[] = {
  "%H",
  "%M",
  "%d",
  "%m",
  "%Y"
};
const char* const timeSetLookup[] = {
  "Hour: ",
  "Minute: ",
  "Day: ",
  "Month: ",
  "Year: "
};
const uint16_t rangeUpperTimeSet[] = {
  23,
  59,
  31,
  12,
  3000
};
const uint16_t rangeLowerTimeSet[] = {
  0,
  0,
  1,
  1,
  2022
};

void menu_setTimeVal(uint8_t line, byte identifier) {
  tft.setCursor(14, _LCDML_ADAFRUIT_FONT_H * (line));
  if (line == LCDML.MENU_getCursorPos()) {
    if(LCDML.BT_checkAny()) {
      if(LCDML.BT_checkEnter()) {
        if(LCDML.MENU_getScrollDisableStatus() == 0) {
          LCDML.MENU_disScroll();
          menu_tempTimeConstant = 
            getSpecificString(timeIdentifierLookup[identifier]).toInt();
          timeChangeFlag = true;
        } else {
          LCDML.MENU_enScroll();
          menu_tempTimeStruct = getCurrentTimeStruct();
          switch (identifier) {
            case 0: menu_tempTimeStruct.tm_hour = menu_tempTimeConstant; break;
            case 1: menu_tempTimeStruct.tm_min = menu_tempTimeConstant; break;
            case 2: menu_tempTimeStruct.tm_mday = menu_tempTimeConstant; break;
            case 3: menu_tempTimeStruct.tm_mon = menu_tempTimeConstant; break;
            case 4: menu_tempTimeStruct.tm_year = menu_tempTimeConstant; break;
          }
          setTime(menu_tempTimeStruct);
          timeChangeFlag = false;
        }
      }

      if (LCDML.BT_checkQuit()) {
        LCDML.MENU_enScroll();
        timeChangeFlag = false;
      }
      if(LCDML.BT_checkUp()) {
        menu_tempTimeConstant++;
        if (menu_tempTimeConstant > rangeUpperTimeSet[identifier])
            menu_tempTimeConstant = rangeLowerTimeSet[identifier];
      }

      if(LCDML.BT_checkDown()) {
        menu_tempTimeConstant--;
        if (menu_tempTimeConstant < rangeLowerTimeSet[identifier])
            menu_tempTimeConstant = rangeUpperTimeSet[identifier];
      }
    }
  timeChangeFlag ? 
      tft.println(timeSetLookup[identifier] + (String)menu_tempTimeConstant) :
      tft.println(timeSetLookup[identifier] + getSpecificString(timeIdentifierLookup[identifier]));
  } else {
    tft.println(timeSetLookup[identifier] + getSpecificString(timeIdentifierLookup[identifier]));
  }
}

void menu_setHour(uint8_t line) {
    menu_setTimeVal(line, 0);
}

void menu_setMinute(uint8_t line){ 
    menu_setTimeVal(line, 1);
}

void menu_setDay(uint8_t line){ 
    menu_setTimeVal(line, 2);
}

void menu_setMonth(uint8_t line){ 
    menu_setTimeVal(line, 3);
}

void menu_setYear(uint8_t line){ 
    menu_setTimeVal(line, 4);
}

//EOF