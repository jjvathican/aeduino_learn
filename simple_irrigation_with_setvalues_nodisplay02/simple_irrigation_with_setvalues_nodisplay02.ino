
#include <MD_DS3231.h>
#include <Wire.h>
#include <Metro.h>
Metro speakerMetro = Metro(5000);
volatile boolean  bShowTime = false;
int relay=10;
int lowvolt=3;
int lowwater=4;
int lowflow=5;
int speaker=13;
long setcount=0;
long setstart=0;
int state=0;
void printTime(void)
{

Serial.print(RTC.yyyy);
Serial.print("-");
Serial.print(RTC.mm);
Serial.print("-");
Serial.print(RTC.dd);
Serial.println();
Serial.print(RTC.h);
Serial.print(":");
Serial.print(RTC.m);
Serial.print(":");
Serial.print(RTC.s);
Serial.println();
//  if (RTC.status(DS3231_12H) == DS3231_ON)
//    lcd.print(RTC.pm ? " pm" : " am");
}
int ErrorFun()
{
  if(digitalRead(lowvolt)==0)
  {  
    if(setcount>3)
    {
      setstart=0;
      motoroff();
      speakeron();
      Serial.println("low voltage");
    }
    setcount=setcount+1;
    return 1;
  }
  else if(digitalRead(lowwater)==0)
  {
    
    if(setcount>3)
    {
     setstart=0;
     motoroff();
     speakeron();
     Serial.println("low water");
    }
    setcount=setcount+1;
    return 1;
  }
  else if(digitalRead(lowflow)==0)
  {
    
    if(setcount>3)
    {
     setstart=0;
     motoroff();
     speakeron();
     Serial.println("low flow");
    }
    setcount=setcount+1;
    return 1;
  }
  else 
  {
    return 0;
  }
  
}
void displayUpdate(void)
{
  RTC.readTime();
  printTime();
 if(checktime(7,40,8,0)==1)
  {
   if(ErrorFun()==0)
   {
    if(setstart>3)
    {
    setcount=0; 
    motoron();
    speakeroff(); 
    }
    setstart=setstart+1;
   }
  }
 
  else if(checktime(16,45,17,0)==1)
  { 
   if(ErrorFun()==0)
   {
    if(setstart>2)
    {
    setcount=0;  
    motoron();
    speakeroff();
    } 
    setstart=setstart+1;
   }
  }
  else
  {
    setstart=0;
    setcount=0;
    motoroff();
    speakeroff();
  }
}

void alarmICB(void)
{
  bShowTime = true;  
}
int checktime(int hour1,int minute1,int hour2,int minute2)
{
 
if(hour1<RTC.h && hour2>RTC.h)
  {
    return 1;
  }
  else if(hour1==RTC.h && hour2>RTC.h)
  {
    if(RTC.m>=minute1)
    {
     return 1;
    }
    else
    {
    return 0;    
    }
  }
   else if(hour1<RTC.h && hour2==RTC.h)
  {
    if(RTC.m<minute2)
    {
      return 1;
    }
    else
    {
      return 0;
    }
  }
  else if(hour1==RTC.h && hour2==RTC.h)
  {
    if(minute1<=RTC.m && minute2>RTC.m)
    {
    return 1;
    }
    else
    {
    return 0;
    }
  }
}
void setup()
{
  Serial.begin(9600);
  digitalWrite(lowvolt,HIGH);
  digitalWrite(lowwater,HIGH);
  digitalWrite(lowflow,HIGH);
  pinMode(relay,OUTPUT);
  pinMode(speaker,OUTPUT);
  pinMode(lowvolt,OUTPUT);
  pinMode(lowwater,OUTPUT);
  pinMode(lowflow,OUTPUT);
// RTC.h=21;
// RTC.m=22;
// RTC.dd=16;
// RTC.mm=3;
// RTC.yyyy=2019;
// RTC.writeTime();

RTC.control( DS3231_12H, DS3231_OFF);

RTC.setAlarm1Callback(alarmICB);
RTC.setAlarm1Type(DS3231_ALM_SEC);
}

void loop()
{

  RTC.checkAlarm1();
  if (bShowTime) 
  {
    displayUpdate();
  }    
  bShowTime = false;
//  if (speakerMetro.check() == 1 and state==1) 
//  {
//  tone(speaker,5000, 2000);
//  }
}
void motoron()
{
  if(state==0)
  {
    // tone(speaker,5000, 10000);
    speakeron();
    delay(3000);
    speakeroff();
  }
  state=1;
 Serial.println("MOTOR ON");
 digitalWrite(relay,HIGH);
}
void motoroff()
{
  state=0;
  Serial.println("MOTOR OFF");
    digitalWrite(relay,LOW);
}
void speakeron()
{
 Serial.println("SPEAKER ON");
    digitalWrite(speaker,HIGH);
}
void speakeroff()
{
  Serial.println("SPEAKER OFF");
    digitalWrite(speaker,LOW);
}
