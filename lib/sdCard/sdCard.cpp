#include "sdCard.h"

SPIClass hspi = SPIClass(HSPI);
boolean isAvailable = false;

void setup_SDCard() {
  SD.end();
    //pinMode(SD_CS, OUTPUT);
    //digitalWrite(SD_CS, HIGH);

    //hspi = SPIClass(HSPI);
    sdCard_open();
}


byte sdCard_open() {
  if (isAvailable) return 2;
  hspi.begin(SPI_SCK, SPI_MISO, SPI_MOSI, SD_CS);
  if(!SD.begin(SD_CS, hspi, 4000000U)) {
    Serial.println("Error accessing microSD card!");
    return 0;
  }
  isAvailable = true;
  return 1;
}

byte sdCard_close() {
  if (!isAvailable) return 2;
  SD.end();
  hspi.end();
  isAvailable = !isAvailable;
  return 1;
}

byte sdCard_reset() {
  sdCard_close();
  return sdCard_open();
}



void printDirectory(File dir, int numTabs) {
  while (true) {

    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}

LinkedList<String> findFilesInDirectory(File dir) {
  LinkedList<String> myLinkedList;
  while (true) {

    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      return myLinkedList;
    }
    myLinkedList.add(entry.name());
    entry.close();
  }
}

bool getWLANInformation(String filename, String &SSID, String &pw) {
  Serial.println(filename);
  File entry = SD.open(filename);
  if (entry.available()) {
    SSID = entry.readStringUntil('\n');
    SSID = SSID.substring(5);
    SSID.trim();
    pw = entry.readStringUntil('\n');
    pw = pw.substring(3);
    pw.trim();
    entry.close();
    return true;
  }
  return false;
}


void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
  Serial.printf("Listing directory: %s\n", dirname);

  File root = fs.open(dirname);
  if(!root){
    Serial.println("Failed to open directory");
    return;
  }
  if(!root.isDirectory()){
    Serial.println("Not a directory");
    return;
  }

  File file = root.openNextFile();
  while(file){
    if(file.isDirectory()){
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if(levels){
        listDir(fs, file.name(), levels -1);
      }
    } else {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("  SIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
}

void writeFile(fs::FS &fs, const char * path, const char * message){
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if(!file){
    Serial.println("Failed to open file for writing");
    return;
  }
  if(file.print(message)){
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  file.close();
}

//EOF