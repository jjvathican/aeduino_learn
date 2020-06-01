
#include <Wire.h>
#include "RTClib.h"
#define DS3231_I2C_ADDRESS 0x68
RTC_DS1307 RTC;
#include <SD.h>                      // need to include the SD library
//#define SD_ChipSelectPin 53  //example uses hardware SS pin 53 on Mega2560
#define SD_ChipSelectPin 4  //using digital pin 4 on arduino nano 328, can use other pins
#include <TMRpcm.h>           //  also need to include this library...
#include <SPI.h>

TMRpcm tmrpcm;
int mon=0;
int mnk=0;
int spk=6;
int led=3;
void setup () {
  tmrpcm.speakerPin = 9; //5,6,11 or 46 on Mega, 9 on Uno, Nano, etc

  
  if (!SD.begin(SD_ChipSelectPin)) {  // see if the card is present and can be initialized:
    Serial.println("SD fail");  
    return;   // don't do anything more if not
  }
  tmrpcm.play("xyz.wav");
    Serial.begin(9600);
    Wire.begin();
    RTC.begin();
    pinMode(led,OUTPUT);
    pinMode(spk,OUTPUT);
   //  RTC.adjust(DateTime(__DATE__, __TIME__));
   digitalWrite(spk,LOW);
  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }
}
void loop () {
    DateTime now = RTC.now(); 
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println(); 
    //digitalWrite(spk,HIGH);
    if(now.hour()==6)
    {
      if(now.minute()>30 && now.minute()<45)
      {
        tmrpcm.play("xyz.wav");
      }
    }
    if(now.hour()!=mon)
    {
      mon=now.hour();
      for(int i=1;i<=(now.hour()%12);i++)
      {
      digitalWrite(spk,HIGH);
      delay(500);
       digitalWrite(spk,LOW);
       delay(500);
      }
    }
     if(now.minute()!=mnk)
    {
      mnk=now.minute();
      for(int i=1;i<=now.minute();i++)
      {
      digitalWrite(led,HIGH);
      delay(200);
      }
      digitalWrite(led,LOW);
    }
    delay(1000);
}
