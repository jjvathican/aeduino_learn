
#include <MD_DS3231.h>
#include <Wire.h>


volatile boolean  bShowTime = false;
int relay=10;
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

void displayUpdate(void)
{
  RTC.readTime();
  printTime();
  if(checktime(7,45,8,0)==1)
  {
    motoron(); 
  }
  else if(checktime(4,0,4,15)==1)
  {
    motoron(); 
  }
  else
  {
    motoroff();
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
  pinMode(relay,OUTPUT);
// RTC.h=4;
// RTC.m=14;
// RTC.dd=15;
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
}
void motoron()
{
 Serial.println("MOTOR ON");
    digitalWrite(relay,HIGH);
}
void motoroff()
{
  Serial.println("MOTOR OFF");
    digitalWrite(relay,LOW);
}
