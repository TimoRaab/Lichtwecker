#include "sdCard.h"

SPIClass * hspi = NULL;

void setup_SDCard() {
    pinMode(SD_CS, OUTPUT);
    digitalWrite(SD_CS, HIGH);

    hspi = new SPIClass(HSPI);
    hspi->begin(SPI_SCK, SPI_MISO, SPI_MOSI, SD_CS);
    //sdSPI.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);
    // Start microSD Card
    Serial.println("Here");
    if(!SD.begin(SD_CS, *hspi))
    {
      Serial.println("Error accessing microSD card!");
      while(true); 
    }
    Serial.println("microSD-Card found.");
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