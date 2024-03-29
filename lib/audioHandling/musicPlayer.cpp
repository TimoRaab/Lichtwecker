#include "musicPlayer.h"

// I2S Connections
#define I2S_DOUT      25
#define I2S_BCLK      26
#define I2S_LRC       27

Audio myMusicPlayer;
int8_t currentVolume = 12;


void setup_musicPlayer() {
    // Setup I2S 
    myMusicPlayer.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
    
    // Set Volume
    myMusicPlayer.setVolume(currentVolume);
    
    // Open music file
    //myMusicPlayer.connecttoFS(SD,"/WakeupTune/MYMUSIC.mp3");
}

bool changeVolume(int8_t step) {
    currentVolume += step;
    if (currentVolume == -1) currentVolume = 0;
    if (currentVolume == 22) currentVolume = 21;
    myMusicPlayer.setVolume(currentVolume);
    return true;
}

bool setVolume(uint8_t volume) {
    currentVolume = volume;
    myMusicPlayer.setVolume(currentVolume);
    return true;
}

bool setCurrentMusic(const char* fname) {
    myMusicPlayer.connecttoFS(SD, fname);
    myMusicPlayer.setAudioPlayPosition(0);
    return true;
}

void playMusic() {
    myMusicPlayer.loop();
}

void stopMusic() {
    myMusicPlayer.stopSong();
}

byte isSongFinished() {
    uint32_t fDuration = myMusicPlayer.getAudioFileDuration();
    if (!fDuration) return 11;
    uint32_t fCurrent = myMusicPlayer.getAudioCurrentTime();
    if (fCurrent == fDuration) return 1;
    return 0;
}