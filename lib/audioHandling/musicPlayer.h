
#ifndef musicPlayer_h
#define musicPlayer_h

#include <Audio.h>

void setup_musicPlayer();

bool changeVolume(int8_t step);
bool setVolume(uint8_t volume);

#endif
//EOF