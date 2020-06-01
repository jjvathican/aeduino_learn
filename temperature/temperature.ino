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
#include <AccelStepper.h>
#include <Metro.h> 
#include <EEPROM.h>

// Define Constants

// Define step constants
#define FULLSTEP 4
#define HALFSTEP 8
#define motorPin1  3    
#define motorPin2  8    
#define motorPin3  12   
#define motorPin4  13  

AccelStepper stepper1(HALFSTEP, motorPin1, motorPin3, motorPin2, motorPin4);
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
unsigned long times=0;
unsigned long hh=0;
unsigned long times1=0;
int hfan=10;
int fan=6;
int distsensor=4;
int bulb=9;
int hbulb=7;
//int k=0;
int sensorValue = 0;    
//int count=0;
//char charBuf[100];
float s=0;
//int f=0;
byte second1=0;
byte minute1=0;
byte hour1=0;
byte dayOfWeek1=0;
byte dayOfMonth1=0;
byte month1=0;
byte year1=0;
int difcount=0; 
int increment=1;
float correction=0.0;
int tscount=0;
int okstate=0;
  int seton=0;
  int sethumon=0;
    int px=0;
    int peak=0; 
 float t=0;
 float tem1=0;
 float tem2=0;
 float t1=0;
 float t2=0;
 int dd=0;
float set_temp=0;
float set_humid = 0;
float relat_h=0;
int mkmove=3000;
Metro metrotime = Metro(5000);
Metro metrotem1 = Metro(3000);
Metro metrotem2 = Metro(4000);
Metro metrotavg = Metro(6000);
Metro metrotdif = Metro(7000);
Metro metrobulb = Metro(8000);
Metro metrohumid = Metro(9000);
Metro metrohdif = Metro(10000);
Metro metrohbulb = Metro(11000);
Metro metromotor = Metro(400000);

Metro metrodisplay = Metro(3000);
Metro metrohfan = Metro(12000);
Metro metrofan = Metro(12000); 

Metro metrotoday = Metro(14000);
Metro metrocorr = Metro(15000);
Metro metrodsensor = Metro(16000);

long EEPROMReadlong(long address) {
  long four = EEPROM.read(address);
  long three = EEPROM.read(address + 1);
  long two = EEPROM.read(address + 2);
  long one = EEPROM.read(address + 3);
  
  return ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);
}

void EEPROMWritelong(int address, long value) {
  byte four = (value & 0xFF);
  byte three = ((value >> 8) & 0xFF);
  byte two = ((value >> 16) & 0xFF);
  byte one = ((value >> 24) & 0xFF);
  
  EEPROM.write(address, four);
  EEPROM.write(address + 1, three);
  EEPROM.write(address + 2, two);
  EEPROM.write(address + 3, one);
}
int onoff=0;
void setup(void) 
{ 
  Serial.begin(9600);
  onoff= EEPROM.read(0);
  onoff=onoff+1;
  if(onoff>100)
  {
  EEPROM.update(0, 0);  
  }
  else
  {
   EEPROM.update(0, onoff);    
  }
 correction= EEPROMReadlong(1); 
 correction = correction /100;
 stepper1.setMaxSpeed(100.0);
 stepper1.setAcceleration(20.0);
 stepper1.setSpeed(100);
 stepper1.moveTo(4096*20);
    
 Wire.begin();
 lcd.begin();
 lcd.backlight();
 lcd.setCursor(0,0);
 lcd.print("PowerON&OFF : ");
 lcd.setCursor(0,1);
 lcd.print(onoff);
 delay(2000);
 lcd.clear();
 lcd.print("Correction : ");
 lcd.setCursor(0,1);
 lcd.print(correction);
 delay(2000);
 digitalWrite(bulb, HIGH);
 digitalWrite(hbulb, HIGH);
 pinMode(distsensor,INPUT);
 pinMode(bulb,OUTPUT);
 pinMode(fan,OUTPUT);
 pinMode(hbulb,OUTPUT);
 pinMode(hfan,OUTPUT);  
 sensors.begin(); 

  dht.begin();
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  dht.humidity().getSensor(&sensor);
  //delayMS = sensor.min_delay / 1000;
  
WDTcounts=0;
WatchDog::init(WDTint_, 500);
} 
byte decToBcd(byte val){
  return( (val/10*16) + (val%10) );
}
// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val){
  return( (val/16*10) + (val%16) );
}
void setDS3231time(byte *second, byte *minute, byte *hour, byte *dayOfWeek, byte
*dayOfMonth, byte *month, byte *year)
{
  // sets time and date data to DS3231
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set next input to start at the seconds register
  Wire.write(decToBcd(*second)); // set seconds
  
  Wire.write(decToBcd(*minute)); // set minutes
  Wire.write(decToBcd(*hour)); // set hours
  Wire.write(decToBcd(*dayOfWeek)); // set day of week (1=Sunday, 7=Saturday)
  Wire.write(decToBcd(*dayOfMonth)); // set date (1 to 31)
  Wire.write(decToBcd(*month)); // set month
  Wire.write(decToBcd(*year)); // set year (0 to 99)
  Wire.endTransmission();
}
void readDS3231time(byte *second,
byte *minute,
byte *hour,
byte *dayOfWeek,
byte *dayOfMonth,
byte *month,
byte *year){
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set DS3231 register pointer to 00h
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
  // request seven bytes of data from DS3231 starting from register 00h
  *second = bcdToDec(Wire.read() & 0x7f);
  *minute = bcdToDec(Wire.read());
  *hour = bcdToDec(Wire.read() & 0x3f);
  *dayOfWeek = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month = bcdToDec(Wire.read());
  *year = bcdToDec(Wire.read());
}

int setlock=1;
void loop(void) 
{
if (metrotem1.check() == 1) 
{
 sensors.requestTemperatures();
 if(t != DEVICE_DISCONNECTED_C) 
  {
    t1=sensors.getTempCByIndex(0);
    tem1=sensors.getTempCByIndex(0);
    Serial.print("Temperature above:");
    Serial.println(tem1);
    s=s+tem1;
    px=px+1;
  } 
  else
  {
    tem1=0;
    Serial.println("Error reading temperature above!");
  }
}

if (metrotem2.check() == 1) 
{ 
 sensors_event_t event1;
 dht.temperature().getEvent(&event1); 
 if (isnan(event1.temperature)) {
    Serial.println(F("Error reading temperature below!"));
    tem2=0;
  }
  else
  {
    Serial.print("correcting value :");
    Serial.println(correction);
    t2=event1.temperature;
    tem2=event1.temperature+correction;
    Serial.print(F("Temperature below: "));   
    Serial.print(tem2);
    px=px+1;
    s=s+tem2;
    Serial.println(F("°C"));
  }  
}
if (metrotavg.check() == 1) 
{ 
if(px!=0)
{
   t=s/px;
   if(px>10)
   {
    px=0;
    s=0;
   }
}
}
if (metrocorr.check() == 1) 
{ 
if(px>3)
{ 
if(abs(tem1-tem2)>1)
{
tscount=tscount+1; 
setlock=1;      
}
else 
{
 tscount=0; 
 setlock=0;
}
if(tscount==5)
  {
       tscount=0;
       correction=t1-t2;
       EEPROMWritelong(1,(correction*100));
  }
}
}

if (metrohumid.check() == 1) 
{ 
 sensors_event_t event;
 dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) 
  {
    Serial.println(F("Error reading humidity!"));
  }
  else {
    Serial.print(F("Humidity: "));
    Serial.print(event.relative_humidity);
    relat_h=event.relative_humidity;
    Serial.println(F("%"));
  }
}


if (metrotime.check() == 1) 
{  
readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,&year); 
  second1=second;
  minute1=minute;
  hour1=hour;
  dayOfWeek1=dayOfWeek;
  dayOfMonth1=dayOfMonth;
  month1=month;
  year1=year;
}
if ( metrohdif.check() == 1) 
{ 
   if(setlock==0)
  {
    if((set_humid-relat_h)>2)
     {
     sethumon=(set_humid-relat_h)*10;
     }
     else
     {
      sethumon=20;
    } 
  }
} 
if ( metrotdif.check() == 1) 
{ 
   if(setlock==0)
  {
    if((set_temp-t)>2)
     {
     seton=(set_temp-t)*10;
     }
     else
     {
      seton=20;
    } 
  }  
}

if (metrohfan.check() == 1) 
{
   if(setlock==0)
  {
     if(minute1%3==0 && second1<seton)
       {
          digitalWrite(hfan, HIGH);
       } 
       else
       {
          digitalWrite(hfan, LOW);
       } 
  }
}

if (metrohbulb.check() == 1) 
{
   if(setlock==0)
  {
  if(minute1%2==0 && t<=set_temp)
   {  
    if(second1<sethumon)
       { 
        digitalWrite(hbulb, LOW);
       }
       else
       {
         digitalWrite(hbulb, HIGH);
       }
   }
   else
   {
    digitalWrite(hbulb, HIGH);
   }
  }
}
if (metrobulb.check() == 0) 
{
   if(setlock==0)
  {
  if(minute1%2==1 && t<=set_temp)
   {  
    if(second1<seton)
       { 
        digitalWrite(bulb, LOW);
       }
       else
       {
         digitalWrite(bulb, HIGH);
       }
   }
   else
   {
    digitalWrite(bulb, HIGH);
   }
  }
}
if (metrofan.check() == 0) 
{
  if(setlock==0)
  {
 if(t<set_temp)
   {
    digitalWrite(fan, HIGH); 
   }
   else
   {
     digitalWrite(fan, LOW); 
   }
  }
}

if (metrodsensor.check() == 1) 
{ 
  if(setlock==1)
  {

  digitalWrite(fan, HIGH);  
  digitalWrite(hfan,HIGH);
  digitalWrite(bulb, HIGH);
  digitalWrite(hbulb,HIGH);  
 
  }
} 



if (metrodisplay.check() == 1) 
{
 if(setlock==0)
  {  
   to_display(t,set_temp,relat_h,set_humid);
  }
  else
  {
    error_display(tscount);
  }
} 

if (metrotoday.check() == 1) 
{
dd=dayOfMonth1;  
if(dd<23)
{
  set_temp=37.70;  
}
else
{
set_temp = (map(dd, 20, 31, 3770, 3680))/100.00;
}

if(dd<20)
{
  set_humid = 50.00;
}
else
{
set_humid = (map(dd, 15, 31, 5000, 6000))/100.00;
}

}



 if(digitalRead(bulb)==1 && digitalRead(hbulb)==1 )
{ 
  
  if(digitalRead(distsensor)==0 && okstate==0 )
  {
  okstate=1;
  times=millis();
  }
  if((millis()-times)>mkmove  && okstate==1 && digitalRead(distsensor)==0)
  {
    times=millis();
    mkmove=mkmove+5000;
    stepper1.stop();
  }
  if(digitalRead(distsensor)==1)
  {
  okstate=0;
  mkmove=3000;
  }
if(metromotor.check() == 1)
{
   increment=increment*-1; 
  stepper1.moveTo((4096*20)*increment); 
}
stepper1.run();
}





 


// if(digitalRead(bulb)==1 && digitalRead(hbulb)==1 )
//{ 
//  
//  if(digitalRead(distsensor)==0 && okstate==0 )
//  {
//  okstate=1;
//  times=millis();
//  }
//  if((millis()-times)>mkmove  && okstate==1 && digitalRead(distsensor)==0)
//  {
//    //okstate=2;
//    stepper1.moveTo(-(stepper1.currentPosition())); 
//    times=millis();
//    times1=millis();
//    mkmove=mkmove+5000;
//  }
//  if(digitalRead(distsensor)==1)
//  {
//  okstate=0;
//  mkmove=1000;
//  }
//if(stepper1.distanceToGo()==0 && okstate==0 && (millis()-times1)>mkmove)
//  { 
//  stepper1.moveTo(-(stepper1.currentPosition())); 
//  }  
//stepper1.run();
//}
//if(metromotor.check() == 1)
//{
//   increment=increment*-1; 
//  stepper1.moveTo((4096*20)*increment); 
//}
//  
//
//} 
//if(digitalRead(distsensor)==0  )
//  {
//  okstate=1;
//  }
//  if((millis()-times)>2000  && okstate==1 && digitalRead(distsensor)==0)
//  {
// 
//    stepper1.stop();
//  //  stepper1.moveTo(peak);
////  //stepper1.setCurrentPosition(1000);
////    stepper1.moveTo(-(stepper1.currentPosition()+(4096*20)));
////    increment=increment+1; 
////    times=millis();  
////  //  increment=1;
//  }
//  else
//  {
//    if(millis()>10000)
//  { 
//     if((millis()-times1)>(1000*increment))
//  {
//  stepper1.stop();
//  //stepper1.setCurrentPosition(1000);
//  peak=stepper1.currentPosition()*(increment*5);
//  stepper1.moveTo(-(stepper1.currentPosition())*(increment*2));
//  increment=increment+1; 
//  times1=millis();  
//  }
//  }
//  }
//  
////  if(okstate==2)
////  {
//
////  }
//  if(digitalRead(distsensor)==1)
// {
//  okstate=0;
// // increment=1; 
////  if(stepper1.distanceToGo()==0)
////  {
////    stepper1.stop();
////    stepper1.moveTo(-(stepper1.currentPosition())); 
////  }
// }
//  stepper1.run(); 
//
//}
//
//// 
//  //stepper1.setCurrentPosition(1000);
//    stepper1.moveTo(-(stepper1.currentPosition()+(4096*20)));
//    increment=increment+1; 
//    times=millis();  
//  //  increment=1;
 //  hh=millis()+(50000);
//    if(stepper1.currentPosition()>=0)
//    {
//     // stepper1.setCurrentPosition(1000);//inorder to quick stop
//      increment=1; 
//       stepper1.stop();
//      // stepper1.setCurrentPosition(-1000);
//    stepper1.moveTo(-4096*10);
//    }
//    else
//    {
//    //   stepper1.setCurrentPosition(-1000);
//      increment=-1;
//       stepper1.stop();
//     //  stepper1.setCurrentPosition(1000);
//      stepper1.moveTo(4096*10); 
//    }
 // stepper1.stop();
   //  increment=increment+1;
   // stepper1.setCurrentPosition(stepper1.currentPosition());
//  if((millis()-times1)>(10000*increment))
//  {
//  stepper1.stop();
//  //stepper1.setCurrentPosition(1000);
//  stepper1.moveTo(-(stepper1.currentPosition())*(increment*5));
//  increment=increment+1; 
//  times1=millis();  
//  }   
//if(increment==1)
//{
// stepper1.stop(); 
// stepper1.moveTo(-(stepper1.currentPosition()-300)); 
//  // Stop as fast as possible: sets new target
////  stepper1.runToPosition();
////   stepper1.stop();  
////  stepper1.moveTo(-(4096*3));
//}
//else if(increment==-1)
//{
//   stepper1.stop();
// stepper1.moveTo(-(stepper1.currentPosition()+300)); 
//  // Stop as fast as possible: sets new target
////  stepper1.runToPosition(); 
////  stepper1.stop(); 
////   stepper1.moveTo((4096*3));
//}
//}

//if((millis()-times1)>(1000*increment))
//  {
//  stepper1.stop();
//  //stepper1.setCurrentPosition(1000);
//  stepper1.moveTo(-(stepper1.currentPosition())*(increment*5));
//  increment=increment+1; 
//  times1=millis();  
//  } 

//if(digitalRead(distsensor)==0)
//{
//  stepper1.moveTo(-stepper1.currentPosition()*(increment*5));
//}

WDTcounts=0;
}   



void WDTint_() 
{                                            
WDTcounts=WDTcounts+1;
if(WDTcounts>20)
{
    WDTcounts=0;
    resetFunc();
}
}
void error_display(int err)
{
 lcd.clear();
 lcd.setCursor(0,0);
 lcd.print("COUNT: ");
 lcd.print(err);
}
void to_display(float te,float set_it,float humid,float set_hum)
{
  lcd.clear();
    if(int(second1/5)%2==0)
    {
    lcd.setCursor(0,0);
    lcd.print("Temp:");
    lcd.print(set_it);
    lcd.print(":");
    lcd.print(te);
    }
    else
    {
    
    lcd.setCursor(0,0);
    lcd.print("Humi:");
    lcd.print(set_hum);
    lcd.print(":");
    lcd.print(humid); 
    }
    lcd.setCursor(0,1);
  int hou;
  String pre="";
  if(hour<12)
  {
    pre="AM";
  }
  else
  {
    pre="PM";
  }
  if(hour%12==0)
  {
    hou=12;
  }
  else
  {
    hou=hour%12;
  }
  lcd.print(hou);
  lcd.print(":");
  lcd.print(minute);
  lcd.print(":");
  lcd.print(second);
  lcd.print(" ");
  lcd.print(dayOfMonth);
  lcd.print("/");
  lcd.print(month);
  lcd.print("/");
  lcd.print(year); 

}

