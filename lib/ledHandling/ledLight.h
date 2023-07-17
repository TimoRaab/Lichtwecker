#ifndef ledLIGHT_h
#define ledLIGHT_h

#include <FastLED.h>

void setup_LED();
void updateLED();
void changeBrightness(byte paraBrightness);

void FillLEDsFromPaletteColors(uint8_t colorIndex);


#endif