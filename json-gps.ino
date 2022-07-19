#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>

// Choose two Arduino pins to use for software serial
int RXPin = 2;
int TXPin = 3;

int GPSBaud = 9600;

// Create a TinyGPS++ object
TinyGPSPlus gps;

// Create a software serial port called "gpsSerial"
SoftwareSerial gpsSerial(RXPin, TXPin);

void setup()
{
  // Start the Arduino hardware serial port at 9600 baud
  Serial.begin(9600);

  // Start the software serial port at the GPS's default baud
  gpsSerial.begin(GPSBaud);
}

void loop()
{
  // This sketch displays information every time a new sentence is correctly encoded.
  while (gpsSerial.available() > 0)
    if (gps.encode(gpsSerial.read()))
      displayInfo();

  // If 5000 milliseconds pass and there are no characters coming in
  // over the software serial port, show a "No GPS detected" error
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println("No GPS detected");
    while(true);
  }
}
////////////////////////////////////////////////////////////////////////////////////////////////
DynamicJsonDocument gpstime(1024);


////////////////////////////////////////////////////////////////////////////////////////////////
void displayInfo()
{
  if (gps.location.isValid())
  {
    gpstime["Latitude"] = (gps.location.lat(), 6);
    gpstime["Longitude"] = (gps.location.lng(), 6);
  }
  else
  {
    gpstime["Latitude"] = ("Location: Not Available");
    gpstime["Longitude"] = ("Location: Not Available");
  }
//    strcat(zero_hour, (const char*) gps.time.hour());

  String zero = "0";
  String hour, minute, second;
  if (gps.time.hour() < 10) {
    hour = zero + String(gps.time.hour());
  }
  else{
    hour = String(gps.time.hour());
  }

  if (gps.time.minute() < 10) {
    minute = zero + String(gps.time.minute());
  }
  else{
    minute = String(gps.time.minute());
  }

  if (gps.time.second() < 10) {
    second = zero + String(gps.time.second());
  }
  else{
    second = String(gps.time.second());
  }

String time;
  if (gps.date.isValid() && gps.time.isValid()){
    time = String(gps.date.year()) + "-" + String(gps.date.month()) + "-" + String(gps.date.day()) + " " + hour + ":" + minute + ":" + second;
    gpstime["Time"]   = time;//("%d-%d-%d %s:%s:%s\n", gps.date.year(), gps.date.month(), gps.date.day(), hour, minute, second);
  }
  else{
    Serial.println("Not Available");
  }
  delay(1000);
  serializeJson(gpstime, Serial);
}
// This prints:
// {"sensor":"gps","time":1351824120,"data":[48.756080,2.302038]}
