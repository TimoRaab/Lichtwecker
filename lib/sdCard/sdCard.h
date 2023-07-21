
#ifndef sdCard_h
#define sdCard_h

#include <SPI.h>
#include <SD.h>
#include <FS.h>
#include <Arduino.h>
#include <LinkedList.h>

#define SD_CS         15
#define SPI_MOSI      12 
#define SPI_MISO      13
#define SPI_SCK       14

void setup_SDCard();
byte sdCard_open();
byte sdCard_close();
byte sdCard_reset();

void printDirectory(File dir, int numTabs);
LinkedList<String> findFilesInDirectory(File dir);
bool getWLANInformation(String filename, String &SSID, String &pw);
void listDir(fs::FS &fs, const char * dirname, uint8_t levels);
void writeFile(fs::FS &fs, const char * path, const char * message);

#endif
//EOF