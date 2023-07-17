
#ifndef musicPlayer_h
#define musicPlayer_h

#include <Audio.h>

void setup_musicPlayer();

bool changeVolume(int8_t step);
bool setVolume(uint8_t volume);
bool setCurrentMusic(const char* fname);

void playMusic();
void stopMusic();

//returns 0 if song is not finished
//returns 1 if song is finished
//returns 11 if audio file duration is 0
byte isSongFinished();

#endif
//EOF