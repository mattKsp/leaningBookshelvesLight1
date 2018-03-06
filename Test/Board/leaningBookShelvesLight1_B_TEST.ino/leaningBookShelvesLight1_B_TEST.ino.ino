/*
 * ESP8266 Wifi With Arduino Uno and Nano
 * by Abhinaba Basu January 23, 2016
 * 
 * https://blogs.msdn.microsoft.com/abhinaba/2016/01/23/esp8266-wifi-with-arduino-uno-and-nano/
 */

#include <SoftwareSerial.h>
SoftwareSerial softSerial(8, 9); // RX, TX

void setup() 
{
  uint32_t baud = 9600;
  Serial.begin(baud);
  softSerial.begin(baud);
  Serial.print("SETUP!! @");
  Serial.println(baud);
}

void loop() 
{
    while(softSerial.available() > 0) 
    {
      char a = softSerial.read();
      if(a == '\0')
        continue;
      if(a != '\r' && a != '\n' && (a < 32))
        continue;
      Serial.print(a);
    }
    
    while(Serial.available() > 0)
    {
      char a = Serial.read();
      Serial.write(a);
      softSerial.write(a);
    }
}
