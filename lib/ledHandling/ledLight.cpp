#include "ledLight.h"


#define LED_PIN_8     32
#define NUM_LEDS_8    10 
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
CRGB leds_8[NUM_LEDS_8];
uint8_t brightness = 90;
#define UPDATES_PER_SECOND 100
CRGBPalette16 currentPalette;
TBlendType    currentBlending;


void setup_LED() {
    delay(1000);
    FastLED.addLeds<LED_TYPE, LED_PIN_8, COLOR_ORDER>(leds_8, NUM_LEDS_8).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(brightness);
    currentPalette = RainbowColors_p;
    currentBlending = LINEARBLEND;
}

void updateLED() {
    static uint8_t startIndex = 0;
    startIndex = startIndex + 5; /* motion speed */
    FillLEDsFromPaletteColors(startIndex);
    FastLED.show();
    //FastLED.delay(1000 / UPDATES_PER_SECOND);
}

void changeBrightness(byte paraBrightness) {
    FastLED.setBrightness(paraBrightness);
}


void FillLEDsFromPaletteColors(uint8_t colorIndex) {
    for (int i=0; i < NUM_LEDS_8; i++) {
        leds_8[i] = ColorFromPalette(currentPalette, colorIndex, brightness, currentBlending);
        colorIndex +=1;
    }
}

//EOF
