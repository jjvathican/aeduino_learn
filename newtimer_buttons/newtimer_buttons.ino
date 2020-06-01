#include "Wire.h"
#define DS3231_I2C_ADDRESS 0x68
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
#include "SD.h"
#define SD_ChipSelectPin 4
#include "TMRpcm.h"
#include "SPI.h"

TMRpcm tmrpcm;
int choice=8;
int up=7;
int down=6;
int mode=5;
int relay=10;
int waitfor=1;
int count=0;
int k=0;
int letcheck=0;
char charBuf[50];
int second1=0;
int minute1=0;
int hour1=0;
int dayOfWeek1=0;
int dayOfMonth1=0;
int month1=0;
int year1=0; 
// Convert normal decimal numbers to binary coded decimal
byte decToBcd(byte val){
  return( (val/10*16) + (val%10) );
}
// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val){
  return( (val/16*10) + (val%16) );
}
void setup(){
  tmrpcm.speakerPin = 9;
Serial.begin(115200);
if (!SD.begin(SD_ChipSelectPin)) {
Serial.println("SD fail");
return;
}
  pinMode(choice,INPUT);
  pinMode(up,INPUT);
  pinMode(down,INPUT);
  pinMode(mode,INPUT);
  pinMode(relay,OUTPUT);
  digitalWrite(relay,LOW);
  Wire.begin();
  Serial.begin(9600);
  lcd.begin();

  // Turn on the blacklight and print a message.
  lcd.backlight();
  lcd.print("Hello, world!");
  // set the initial time here:
  // DS3231 seconds, minutes, hours, day, date, month, year
  setDS3231time(55,7,16,2,17,6,19);
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
void playalarm(int second,int waittime)
{
  if(second<=waittime)
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
      }
      delay(1000);
     
}
void song_ending_alarm()
{
     delay(500);
     digitalWrite(relay, HIGH);
     delay(2000);
     digitalWrite(relay, LOW);
}
void fralarm(int hour,int minute,int second)
{
  if(hour==8)
 {
   if(minute==55)
   {
    playalarm(second,waitfor+2);
   }
 }
 if(hour==9) 
 { 
   if(minute==0)
   {
    playalarm(second,waitfor+2);
    if(second==5)
    { 
     music_play("xyz.wav");
     song_ending_alarm();
    }
   }
   
   
   if(minute==55)
   {
    playalarm(second,waitfor);
   }
 }

if(hour==10) 
 {
  
   if(minute==50)
   {
    playalarm(second,waitfor+1);
   }
 }

if(hour==11) 
 {
   if(minute==0)
   {
    playalarm(second,waitfor+1);
   }
   if(minute==50)
   {
    playalarm(second,waitfor);
   }
 }
 
 if(hour==12) 
 {
  
   if(minute==40)
   {
    playalarm(second,waitfor+1);
   }
 }
 if(hour==14) 
 {
  if(minute==0)
   {
     playalarm(second,waitfor+1);
   }
   if(minute==40)
   {
    playalarm(second,waitfor);
   }
 }
  if(hour==15) 
 {
   if(minute==20)
   {
    playalarm(second,waitfor+1);
   }
   if(minute==30)
   {
    playalarm(second,waitfor+1);
   }
 }
 if(hour==16) 
 {
   if(minute==8)
   {
    playalarm(second,1);
    if(second==5)
    { 
     music_play("jana.wav");
     song_ending_alarm();
    }
   }
  
 
 }
}

void alarm(int hour,int minute,int second)
{
 if(hour==8)
 {
   if(minute==55)
   {
    playalarm(second,waitfor+2);
   }
 }
 if(hour==9) 
 { 
   if(minute==0)
   {
    playalarm(second,waitfor+2);
    if(second==5)
    { 
     music_play("shreya.wav");
     song_ending_alarm();
    }
   }
   
   
   if(minute==55)
   {
    playalarm(second,waitfor);
   }
 }

if(hour==10) 
 {
  
   if(minute==50)
   {
    playalarm(second,waitfor+1);
   }
 }

if(hour==11) 
 {
   if(minute==0)
   {
    playalarm(second,waitfor+1);
   }
   if(minute==50)
   {
    playalarm(second,waitfor);
   }
 }
 
 if(hour==12) 
 {
  
   if(minute==40)
   {
    playalarm(second,waitfor+1);
   }
 }
  if(hour==13) 
 {
  
   if(minute==30)
   {
    playalarm(second,waitfor+1);
   }
 }
   if(hour==14) 
 {
  
   if(minute==20)
   {
    playalarm(second,waitfor);
   }
 }
    if(hour==15) 
 {
  
   if(minute==10)
   {
    playalarm(second,waitfor+1);
   }
   if(minute==20)
   {
    playalarm(second,waitfor+1);
   }
 }
   if(hour==16) 
 {
   if(minute==8)
   {
    playalarm(second,1);
    if(second==5)
    { 
     music_play("jana.wav");
     song_ending_alarm();
    }
   }
  
 
 }
}
void displayTime()
{
 
  lcd.clear();
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  // retrieve data from DS3231
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,
  &year);
  second1=second;
  minute1=minute;
  hour1=hour;
  dayOfWeek1=dayOfWeek;
  dayOfMonth1=dayOfMonth;
  month1=month;
  year1=year;
  // send it to the serial monitor
  Serial.print(hour, DEC);
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
  // convert the byte variable to a decimal number when displayed
  Serial.print(":");
  Serial.print(minute, DEC);
  if (minute<10) lcd.print('0');
  lcd.print(minute,DEC);
  lcd.print(":");
  Serial.print(":");
  Serial.print(second, DEC);
  if (second<10) lcd.print('0');
  lcd.print(second,DEC);
  lcd.print(" ");
  lcd.print(pre);

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


  
  //lcd.clear();
  //lcd.setCursor(count,0);
  String stringOne = "";
  
    stringOne=stringOne+"                ";
    stringOne=stringOne+String(dayOfMonth);
    stringOne=stringOne+"/";
    stringOne=stringOne+String(month);
    stringOne=stringOne+"/";
    stringOne=stringOne+String(year);
     stringOne=stringOne+"(";
    switch(dayOfWeek){
  case 1:
    stringOne=stringOne+"Sunday";
    break;
  case 2:
    stringOne=stringOne+"Monday";
    break;
  case 3:
    stringOne=stringOne+"Tuesday";
    break;
  case 4:
    stringOne=stringOne+"Wednesday";
    break;
  case 5:
    stringOne=stringOne+"Thursday";
    break;
  case 6:
    stringOne=stringOne+"Friday";
    break;
  case 7:
    stringOne=stringOne+"Saturday";
    break;
  }
   stringOne=stringOne+")";
  stringOne=stringOne+" JJ Bell System ";
    stringOne=stringOne+"#Contact:9544414811  ";
  stringOne=stringOne+"                 ";
    stringOne.toCharArray(charBuf, 100) ;
  lcd.setCursor(count,1);
  
   for(int j=k;j<=15+k;++j)
  {
    lcd.print(charBuf[j]);
   // Serial.print(charBuf[j]);
  }
  k=k+1;
  //Serial.println();
    if(k+16>strlen(charBuf))
    {
      k=0;
    }
     
  
  Serial.print(" ");
  Serial.print(dayOfMonth, DEC);
  Serial.print("/");
  Serial.print(month, DEC);
 
   
  Serial.print("/");
  Serial.print(year, DEC);
  Serial.print(" Day of week: ");
  switch(dayOfWeek){
  case 1:
    Serial.println("Sunday");
    break;
  case 2:
    Serial.println("Monday");
    break;
  case 3:
    Serial.println("Tuesday");
    break;
  case 4:
    Serial.println("Wednesday");
    break;
  case 5:
    Serial.println("Thursday");
    break;
  case 6:
    Serial.println("Friday");
    break;
  case 7:
    Serial.println("Saturday");
    break;
  }
}
int km=0;
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
    switch(km)
    {
      case 1:
          lcd.print("minute :");
          lcd.print(minute1);
    }
 
}
void loop(){
  Serial.println(digitalRead(5));
   displayTime(); 
   if(digitalRead(mode)==1)
  {
    displayTime(); 
  }
  else
  {
   // TimeAdjust();
  }
  delay(1000); 
}
