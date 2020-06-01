#include "Wire.h"
#define DS3231_I2C_ADDRESS 0x68
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
#include "SD.h"
#define SD_ChipSelectPin 4
#include "TMRpcm.h"
#include "SPI.h"
#include <WatchDog.h>
byte WDTcounts;
void(* resetFunc) (void) = 0;
TMRpcm tmrpcm;
int choice=6;
int up=5;
int down=3;
int mode=7;
int rev=A0;
int relay=2;
int waitfor=1;
int count=0;
int k=0;
int letcheck=0;
int second1=0;
int minute1=0;
int hour1=0;
int dayOfWeek1=0;
int dayOfMonth1=0;
int month1=0;
int year1=0; 
int amplifier=8;
//char charBuf[50];
// Convert normal decimal numbers to binary coded decimal
byte decToBcd(byte val){
  return( (val/10*16) + (val%10) );
}
// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val){
  return( (val/16*10) + (val%16) );
}
void setup()
{
  tmrpcm.speakerPin = 9;
//Serial.begin(115200);
if (!SD.begin(SD_ChipSelectPin)) {
Serial.println("SD fail");
return;
}
    pinMode(rev,INPUT);
  pinMode(choice,INPUT);
  pinMode(up,INPUT);
  pinMode(down,INPUT);
  pinMode(mode,INPUT);
  pinMode(amplifier,OUTPUT); 
  pinMode(relay,OUTPUT);
  digitalWrite(relay,LOW);
  digitalWrite(amplifier,LOW);
  Wire.begin();
  Serial.begin(9600);
  lcd.begin();

  // Turn on the blacklight and print a message.
  lcd.backlight();
  lcd.print("<<vivek & team>>");
  delay(5000);
  WDTcounts=0;
  WatchDog::init(WDTint_, 500);
  // set the initial time here:
  // DS3231 seconds, minutes, hours, day, date, month, year
 setDS3231time(0,56,8,2,5,3,19);
//  RTC.adjust(DateTime(__DATE__, __TIME__));
}
void setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte
dayOfMonth, byte month, byte year){
  // sets time and date data to DS3231
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set next input to start at the seconds register
  Wire.write(decToBcd(second)); // set seconds
  
  Wire.write(decToBcd(minute)); // set minutes
  Wire.write(decToBcd(hour)); // set hours
  Wire.write(decToBcd(dayOfWeek)); // set day of week (1=Sunday, 7=Saturday)
  Wire.write(decToBcd(dayOfMonth)); // set date (1 to 31)
  Wire.write(decToBcd(month)); // set month
  Wire.write(decToBcd(year)); // set year (0 to 99)
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
void playalarm(int second,int increment,int waittime)
{
  
  if(second>=increment&& second<=(waittime+increment))
  {
  digitalWrite(relay, HIGH);
  lcd.setCursor(0,0);
  lcd.print("(BELL !!)    "); 
  }
  else
  {
    digitalWrite(relay, LOW);
  }
   
}
void music_play(char abc[])
{
      tmrpcm.play(abc);
      while(tmrpcm.isPlaying())
      {
         lcd.setCursor(0,0);
         lcd.print("(PLAYING !!)    "); 
         delay(200);
         WDTcounts=0;
      }
      delay(1000);
     
}
void song_ending_alarm()
{
   lcd.setCursor(0,0);
   lcd.print("(BELL !!)    "); 
     delay(500);
     digitalWrite(relay, HIGH);
     delay(2000);
     digitalWrite(relay, LOW);
}

void fralarm(int hour,int minute,int second)
{
if((hour*60+minute)>=(8*60+58)-1 && (hour*60+minute)<=(8*60+58)+1 || (hour*60+minute)>=(15*60+58)-1 && (hour*60+minute)<=(15*60+58)+1)
 {
   
    digitalWrite(amplifier,HIGH);
    lcd.setCursor(0,1);
    lcd.print("Amplifier_on");
  
 }
 else
 {
    digitalWrite(amplifier,LOW);
   // lcd.clear();
 }

 if(hour==8) 
 { 
   if(minute==58)
   {
    playalarm(second,0,waitfor+2);
    if(second==10)
    { 
     music_play("shreya.wav");
     song_ending_alarm();
    }
   }
 }
 if(hour==9)
 {     
   if(minute==55)
   {
    playalarm(second,0,waitfor);
   }
 }
 

if(hour==10) 
 {
  
   if(minute==50)
   {
    playalarm(second,0,waitfor+1);
   }
 }

if(hour==11) 
 {
   if(minute==0)
   {
    playalarm(second,0,waitfor+1);
   }
   if(minute==50)
   {
    playalarm(second,0,waitfor);
   }
 }
 
 if(hour==12) 
 {
  
   if(minute==40)
   {
    playalarm(second,0,waitfor+1);
   }
 }
  if(hour==14) 
 {
  
   if(minute==0)
   {
    playalarm(second,0,waitfor+1);
   }
    if(minute==40)
   {
    playalarm(second,0,waitfor+1);
   }
 }
 if(hour==15) 
 {
   if(minute==20)
   {
    playalarm(second,0,waitfor);
   }
   if(minute==30)
   {
    playalarm(second,0,waitfor);
   }
    if(minute==58)
   {
    playalarm(second,0,1);
    if(second==10)
    { 
     music_play("jana.wav");
     song_ending_alarm();
    }
   }
 }
 

}

void alarm(int hour,int minute,int second)
{
 if((hour*60+minute)>=(8*60+58)-1 && (hour*60+minute)<=(8*60+58)+1 || (hour*60+minute)>=(15*60+58)-1 && (hour*60+minute)<=(15*60+58)+1)
 {
   
    digitalWrite(amplifier,HIGH);
    lcd.setCursor(0,1);
    lcd.print("Amplifier_on");
  
 }
 else
 {
    digitalWrite(amplifier,LOW);
   // lcd.clear();
 }

 if(hour==8) 
 { 
   if(minute==58)
   {
    playalarm(second,0,waitfor+2);
    if(second==10)
    { 
     music_play("shreya.wav");
     song_ending_alarm();
    }
   }
 }
 if(hour==9)
 {     
   if(minute==55)
   {
    playalarm(second,0,waitfor);
   }
 }
 

if(hour==10) 
 {
  
   if(minute==50)
   {
    playalarm(second,0,waitfor+1);
   }
 }

if(hour==11) 
 {
   if(minute==0)
   {
    playalarm(second,0,waitfor+1);
   }
   if(minute==50)
   {
    playalarm(second,0,waitfor);
   }
 }
 
 if(hour==12) 
 {
  
   if(minute==40)
   {
    playalarm(second,0,waitfor+1);
   }
 }
  if(hour==13) 
 {
  
   if(minute==30)
   {
    playalarm(second,0,waitfor+1);
   }
 }
   if(hour==14) 
 {
  
   if(minute==20)
   {
    playalarm(second,0,waitfor);
   }
 }
    if(hour==15) 
 {
  
   if(minute==10)
   {
    playalarm(second,0,waitfor+1);
   }
   if(minute==20)
   {
    playalarm(second,0,waitfor+1);
   }
   if(minute==58)
   {
    playalarm(second,0,1);
    if(second==10)
    { 
     music_play("jana.wav");
     song_ending_alarm();
    }
   }
 }

}
 byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
void displayTime(){
  lcd.clear();
 
  // retrieve data from DS3231
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,
  &year);
  // send it to the serial monitor
  //second1=100;
  //Serial.println(month1);
  //Serial.print(hour, DEC);
   if(int(second/5)%2==0)
    {
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
 if (hou<10) lcd.print('0');
 lcd.print(hou);
 lcd.print(":");
  if (minute<10) lcd.print('0');
  lcd.print(minute,DEC);
  lcd.print(":");
  if (second<10) lcd.print('0');
  lcd.print(second,DEC);
  lcd.print(" ");
  lcd.print(pre);
    }
    else
    {
      lcd.print("Date :");
   if(dayOfMonth<10) lcd.print('0');
  lcd.print(dayOfMonth);
  lcd.print("/");
  if(month<10) lcd.print('0');
  lcd.print(month);
  lcd.print("/");
  lcd.print(year);
    }
if(dayOfWeek==6)//friday
  {
   fralarm(hour,minute,second); 
  }
else if(dayOfWeek==7)//saturday
  {
   if(dayOfMonth>7&&dayOfMonth<15)
   {
    
   } 
   else
   {
    alarm(hour,minute,second);

   }
  }
else if(dayOfWeek==1)//sunday
{
  
}
else
  {
  alarm(hour,minute,second);
  }

     
}
int km=1;
void TimeAdjust()
{
  lcd.clear();
  lcd.setCursor(0,0);
  if(digitalRead(choice)==1)
  {
    while(digitalRead(choice)==1){}
    //lcd.print(km);
    km=km+1;
  }
   if(digitalRead(rev)==1)
  {
    while(digitalRead(rev)==1){}
    //lcd.print(km);
    km=km-1;
    if(km<1)
    {
     km=1; 
    }
  }
    switch(km)
    {
       case 1:
          lcd.print("second :");
          lcd.print(second1);
          change_time1(&second1,60);
          break;
      case 2:
          
          lcd.print("minute :");
          lcd.print(minute1);
          change_time1(&minute1,60);
          break;
      case 3:
          lcd.print("hour :");
          lcd.print(hour1);
          change_time(&hour1,24); 
          break; 
      case 4:
          lcd.print("dayOfWeek :");
          lcd.print(dayOfWeek1);
          change_time(&dayOfWeek1,7); 
          break;    
      case 5:
          lcd.print("dayOfMonth :");
          lcd.print(dayOfMonth1);
          change_time(&dayOfMonth1,31);
          break;           
      case 6:
          lcd.print("month :");
          lcd.print(month1);
          change_time(&month1,12); 
          break;
      case 7:
          lcd.print("year :");
          lcd.print(year1);
          change_time(&year1,25); 
          break; 
      
      case 8:
          lcd.print("saving.....");
          setDS3231time(second1,minute1,hour1,dayOfWeek1,dayOfMonth1,month1,year1);
          delay(1000);
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("saved...!");
          delay(1000);
          km=1;
          break;               
    }
 
}
void change_time1(int *mm,int tt)
{
           if(digitalRead(up)==1)
          {
           while(digitalRead(up)==1) {}
           *mm=(*mm+1)%tt;     
          }
          if(digitalRead(down)==1)
          {
           while(digitalRead(down)==1) {}
          *mm=(*mm-1)%tt;
          }
         if(*mm<0)
         {
          *mm=tt-1;  
         }
}
void change_time(int *mm,int tt)
{
           if(digitalRead(up)==1)
          {
           while(digitalRead(up)==1) {}
           *mm=(*mm+1)%tt;     
          }
          if(digitalRead(down)==1)
          {
           while(digitalRead(down)==1) {}
          *mm=(*mm-1)%tt;
          }
         if(*mm<=0)
         {
          *mm=tt;  
         }
}
void loop(){
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,
  &year);
  
  if(year==0)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Time Wrong");
    setDS3231time(second1,minute1,hour1,dayOfWeek1, dayOfMonth1, month1,year1);
    delay(2000);
  }
  else
  {
  second1=second; 
  minute1=minute;
  hour1=hour;
  dayOfWeek1=dayOfWeek;
  dayOfMonth1=dayOfMonth;
  month1=month;
  year1=year;
  if(digitalRead(mode)==0)
  {   
    displayTime();
    km=1;
    delay(1000); 
  }
  else
  {
     Serial.println(second1);
     while(digitalRead(mode)!=0)
      {
       TimeAdjust();
       delay(100);
       WDTcounts=0;
      }
   
  } 
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


