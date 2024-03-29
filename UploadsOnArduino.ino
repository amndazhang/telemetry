#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <LoRa.h>

// Choose two Arduino pins to use for software serial
int RXPin = 2;
int TXPin = 3;

int GPSBaud = 9600;
   
// Create a TinyGPS++ object
TinyGPSPlus gps;

// Create a software serial port called "gpsSerial"
SoftwareSerial gpsSerial(RXPin, TXPin);

#define ss 18
#define rst 14
#define dio0 26

void setup()
{
  // Start the Arduino hardware serial port at 9600 baud
  
  Serial.begin(9600);
  while (!Serial); //wait until serial is connected
  Serial.println("LoRa Sender");

   // Change sync word (0xF3) to match the receiver
  // The sync word assures you don't get LoRa messages from other LoRa transceivers
  // ranges from 0-0xFF
  LoRa.setSyncWord(0xF3);
  Serial.println("LoRa Initializing OK!");


  
  // Start the software serial port at the GPS's default baud
  gpsSerial.begin(GPSBaud);
}

void loop()
{
  // This sketch displays information every time a new sentence is correctly encoded.
//  while (gpsSerial.available() > 0)
//    if (gps.encode(gpsSerial.read()))
//      displayInfo();

  // If 5000 milliseconds pass and there are no characters coming in
  // over the software serial port, show a "No GPS detected" error
//  if (millis() > 5000 && gps.charsProcessed() < 10)
//  {
//    Serial.println("No GPS detected");
//    while (true);
//  }

  String gpstime;

  if (gps.location.isValid())
  {
    gpstime += "{Latitude: " + (gps.location.lat(), 6);
    gpstime += ", Longitude: " + (gps.location.lng(), 6);
  }
  else
  {
    gpstime += "{Latitude: Not Available ";
    gpstime += ", Longitude: Not Available ";
  }

  String zero = "0";
  String hour, minute, second;
  if (gps.time.hour() < 10) {
    hour = zero + String(gps.time.hour());
  }
  else {
    hour = String(gps.time.hour());
  }

  if (gps.time.minute() < 10) {
    minute = zero + String(gps.time.minute());
  }
  else {
    minute = String(gps.time.minute());
  }

  if (gps.time.second() < 10) {
    second = zero + String(gps.time.second());
  }
  else {
    second = String(gps.time.second());
  }

  String timeStr;
  if (gps.date.isValid() && gps.time.isValid()) {
    timeStr = String(gps.date.year()) + "-" + String(gps.date.month()) + "-" + String(gps.date.day()) + " " + hour + ":" + minute + ":" + second;
    gpstime += ", Time: " + timeStr + "}";
  }
  else {
    Serial.println("Not Available");
  }
  delay(1000);
  //Serial.println(gpstime);
  //Send LoRa packet to receiver
//  Serial.println(gpstime);
  LoRa.beginPacket();
  LoRa.print(String(gpstime));
  LoRa.endPacket();

  Serial.println("hi");
  
  delay(5000);
}


//void displayInfo() {
//  
//}
