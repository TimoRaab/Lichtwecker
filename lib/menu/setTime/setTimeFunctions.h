#ifndef setTimeFunctions_h
#define setTimeFunctions_h

#include <Arduino.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <LCDMenuLib2.h>  
#include <ButtonDebounce.h>

void menu_setTimeVal(uint8_t line, byte identifier);
void menu_setHour(uint8_t line);
void menu_setMinute(uint8_t line);
void menu_setMonth(uint8_t line);
void menu_setDay(uint8_t line);
void menu_setYear(uint8_t line);
void menu_resetSeconds(uint8_t line);
void menu_setTimeManual(uint8_t line);

#endif
//EOF