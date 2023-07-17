#include <SPI.h>
#include <SD.h>
#include <FS.h>
#include <Arduino.h>
#include <LinkedList.h>

#define SD_CS         15
#define SPI_MOSI      12 
#define SPI_MISO      13
#define SPI_SCK       14

SPIClass hspi = SPIClass(HSPI);

void setup_SDCard() {
    pinMode(SD_CS, OUTPUT);
    digitalWrite(SD_CS, HIGH);

    //hspi = SPIClass(HSPI);
    hspi.begin(SPI_SCK, SPI_MISO, SPI_MOSI, SD_CS);
    if(!SD.begin(SD_CS, hspi))
    {
      Serial.println("Error accessing microSD card!");
      while(true); 
    }
    //Serial.println("microSD-Card found.");
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

void setup(){
    Serial.begin(115200);
    setup_SDCard();
    Serial.println("Startup finished");
    delay(1000);
    Serial.println("delay finished");
    //writeFile(SD, "/hello.txt", "Hello ");

    //listDir(SD, "/", 2);
    Serial.println("First Print");
    Serial.println("\n\n\n");
    File root2 = SD.open("/");
    printDirectory(root2, 2);
    root2.close();
    Serial.println("Second Print");
    Serial.println("\n\n\n");
    File root = SD.open("/WLAN", "r");
    printDirectory(root, 0);
    root.close();
    root = SD.open("/WLAN", "r");
    int tempCounter = 0;
    LinkedList<String> fName = findFilesInDirectory(root);
    while(fName.size() == 0 && tempCounter < 5) {
        delay(100);
        Serial.println("Error File Size!, Try: " + tempCounter);
        tempCounter++;
        fName = findFilesInDirectory(root);
    }

    Serial.println("GetData from LinkedList");
    Serial.println("Finished");
    String SSID = "";
    String pw = "";
    getWLANInformation("/WLAN/" + fName.get(0), SSID, pw);

    Serial.println("Outside");
    Serial.println(SSID);
    Serial.println(pw);
}


void loop(){}