// NOT TESTED

#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>
#include "heltec.h"

/*
   This sample sketch demonstrates the normal use of a TinyGPSPlus (TinyGPSPlus) object.
   It requires the use of SoftwareSerial, and assumes that you have a
   4800-baud serial GPS device hooked up on pins 4(rx) and 3(tx).
*/

#define BAND    915E6

static const int RXPin = 32, TXPin = 33;
static const uint32_t GPSBaud = 9600;

TinyGPSPlus gps; // The TinyGPSPlus object
SoftwareSerial ss(RXPin, TXPin); // The serial connection to the GPS device

int counter = 0;
int startTime = millis();

String zero = "0";

void setup() {
  Heltec.begin(true /*DisplayEnable Enable*/, true /*Heltec.LoRa Disable*/, false /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);
  Serial.begin(115200);
  while(!Serial);
  ss.begin(GPSBaud);
  while(!ss);
}

// displays information every time a new sentence is correctly encoded
void loop() { 
  while (ss.available() > 0)
    if (gps.encode(ss.read()))
      displayInfo();

  // no GPS
  if (millis()>15000 && gps.charsProcessed()<10) {
    Serial.println("No GPS Detected");
    while(true);
  }
  if (millis()>5000 && gps.charsProcessed()<10) {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }

  String gpsLocation = getGPSLocation();
  String hour = getHour();
  String minute = getMinute();
  String second = getSecond();
  String timeStr = makeTimeString();

  // boat 1 = even seconds
  if (gps.time.second()%2 == 0){
    if (Serial.available()){
      LoRa.beginPacket();
      LoRa.setTxPower(14,RF_PACONFIG_PASELECT_PABOOST);
      LoRa.print(gpsLocation + Serial.readStringUntil('\n') + ",1");
      Serial.print(gpsLocation);
      LoRa.endPacket(); 
    }
  }

  // boat 2 = odd seconds
  // if(gps.time.second()%2 == 1){
  //   if(Serial.available()){
  //     LoRa.beginPacket();
  //     LoRa.setTxPower(14,RF_PACONFIG_PASELECT_PABOOST);
  //     LoRa.print(gpsLocation + Serial.readStringUntil('\n') + ",2");
  //     Serial.print(gpsLocation);
  //     LoRa.endPacket(); 
  //   }
  // }

  
}

String getGPSLocation() {
  String loc;
  if (gps.location.isValid()) {
    loc += String(gps.location.lat(), 6); 
    loc += "," + String(gps.location.lng(), 6);
  } else {
    loc += "N/A";
    loc += ",N/A";
  }

  return loc;
}

String getHour() {
  String hr;
  if (gps.time.hour() < 10) {
    hr = zero + String(gps.time.hour());
  } else {
    hr = String(gps.time.hour());
  } 

  return hr;
}

String getMinute() {
  String min;
  if (gps.time.minute() < 10) {
    min = zero + String(gps.time.minute());
  } else {
    min = String(gps.time.minute());
  } 

  return min;
}

String getSecond() {
  String sec;
  if (gps.time.second() < 10) {
    sec = zero + String(gps.time.second());
  } else {
    sec = String(gps.time.second());
  }

  return sec;
}

String makeTimeString() {
  String str;
  if (gps.date.isValid() && gps.time.isValid()) {
    str = String(gps.date.year()) + "-" + String(gps.date.month()) + "-" + String(gps.date.day()) + " " + hour + ":" + minute + ":" + second;
    gpsLocation += "," + str + ",";
  } else {
    Serial.println("NA");
  }

  return str;
}
  
void displayInfo() {
  Serial.print(F("Location: ")); 
  if (gps.location.isValid()) {
    Serial.print(String(gps.location.lat(), 6));
    Serial.print(F(","));
    Serial.print(String(gps.location.lng(), 6));
  } else {
    Serial.print(F("INVALID"));
  }

  Serial.print(F("  Date/Time: "));
  if (gps.date.isValid()) {
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.year());
  } else {
    Serial.print(F("INVALID"));
  }

  Serial.print(F(" "));
  if (gps.time.isValid()) {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(F("."));
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.print(gps.time.centisecond());
  } else {
    Serial.print(F("INVALID"));
  }

  Serial.println();
}
