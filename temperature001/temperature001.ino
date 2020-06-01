// First we include the libraries
#include <OneWire.h> 
#include <Adafruit_Sensor.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include "RTClib.h"
#define DS3231_I2C_ADDRESS 0x68
RTC_DS1307 RTC;
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
#include <DHT.h>
#include <DHT_U.h>
#define DHTTYPE    DHT11   
#define DHTPIN 2
#include <WatchDog.h>
byte WDTcounts;
void(* resetFunc) (void) = 0;
DHT_Unified dht(DHTPIN, DHTTYPE);

uint32_t delayMS;

/********************************************************************/
// Data wire is plugged into pin 2 on the Arduino 
#define ONE_WIRE_BUS 11
/********************************************************************/
// Setup a oneWire instance to communicate with any OneWire devices  
// (not just Maxim/Dallas temperature ICs) 
OneWire oneWire(ONE_WIRE_BUS); 
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
/********************************************************************/
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);
/********************************************************************/ 
int hfan=10;
int fan=6;
int bulb=9;
int hbulb=7;
int k=0;
int sensorValue = 0;    
//int count=0;
//char charBuf[100];
int s=1;
int f=0;

void setup(void) 
{ 
 Serial.begin(9600); 
 Wire.begin();
 digitalWrite(bulb, HIGH);
 digitalWrite(hbulb, HIGH);
 pinMode(bulb,OUTPUT);
 pinMode(fan,OUTPUT);
 pinMode(hbulb,OUTPUT);
 pinMode(hfan,OUTPUT);

 sensors.begin(); 
  dht.begin();
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  dht.humidity().getSensor(&sensor);
  delayMS = sensor.min_delay / 1000;

  
WDTcounts=0;
WatchDog::init(WDTint_, 500);

  
 
} 

void loop(void) 
{ 


 float t=sensors.getTempCByIndex(0);

float set_temp = 37.5;//(map(dd, 15, 31, 3750, 3655))/100.00;
float set_humid =52;// (map(dd, 10, 31, 4800, 6300))/100.00;

 delay(delayMS);
 sensors_event_t event;
 sensors_event_t event1;
 dht.temperature().getEvent(&event1);
 dht.humidity().getEvent(&event);

 
  if (isnan(event1.temperature)) {
    Serial.println(F("Error reading temperature!"));
  }
  else {
    Serial.print(F("Temperature: "));
    Serial.print(event1.temperature);
    Serial.println(F("°C"));
  }
 
Serial.print("Current_Temperature : ");
Serial.println(t);



   digitalWrite(fan, HIGH);
   if(event1.temperature<37.5)//set_temp)
   {
     digitalWrite(hfan, HIGH);
     digitalWrite(bulb,LOW);
     digitalWrite(hbulb, LOW);
    // k=100;
   }
  
   else
   {
     digitalWrite(hfan, LOW);
    digitalWrite(bulb,HIGH);
    digitalWrite(hbulb, HIGH);
    //k=k-1;
   }
  

 

     
WDTcounts=0;
}   



void WDTint_() 
{                                         // watchdog timer interrupt service routine    
WDTcounts=WDTcounts+1;
if(WDTcounts>10)
{
    WDTcounts=0;
    resetFunc();
}
}
