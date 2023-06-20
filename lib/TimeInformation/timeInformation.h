
#ifndef timeInformation_h
#define timeInformation_h

#include <Arduino.h>
#include <WiFi.h>
//#include <ESP32Time.h>


byte setup_TimeInformation();
byte setTimeInformation();

String getDate();
String getTime();




#endif
//EOF