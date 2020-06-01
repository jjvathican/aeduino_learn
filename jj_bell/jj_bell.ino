#include "Wire.h"
#define DS3231_I2C_ADDRESS 0x68
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
int relay=10;
int waitfor=2;
int count=15;
int k=0;
int wait=0;
char charBuf[50];
// Convert normal decimal numbers to binary coded decimal
byte decToBcd(byte val){
  return( (val/10*16) + (val%10) );
}
// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val){
  return( (val/16*10) + (val%16) );
}
void setup(){
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
  setDS3231time(30,7,8,2,6,10,19);
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
  }
  else
  {
    digitalWrite(relay, LOW);
  }
}

void fralarm(int hour,int minute,int second)
 {

if(hour==8) 
 {
  
   if(minute==58)
   {
    playalarm(second,waitfor+1);
   }
 }
  if(hour==9) 
 {
   if(minute==0)
   {
    playalarm(second,waitfor+1);
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
   if(minute==3)
   {
    playalarm(second,waitfor);
   }
   if(minute==5)
   {
    playalarm(second,waitfor+2);
   }
 }
}
void alarm(int hour,int minute,int second)
{
  if(hour==8) 
 {
  
   if(minute==58)
   {
    playalarm(second,waitfor+1);
   }
 }
 if(hour==9) 
 {
   if(minute==0)
   {
    playalarm(second,waitfor+2);
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
   if(minute==3)
   {
    playalarm(second,waitfor+1);
   }
   if(minute==5)
   {
    playalarm(second,waitfor+2);
   }
 
 }
}
void displayTime(){
  
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  // retrieve data from DS3231
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,
  &year);
int second1=0;
int minute1=0;
int hour1=0;
int dayOfWeek1=0;
int dayOfMonth1=0;
int month1=0;
int year1=0; 
if(wait > 10)
{
lcd.setCursor(0,0);
lcd.print("Time Wrong");
delay(100000);
}
if(year==0)
{
  setDS3231time(second1,minute1,hour1,dayOfWeek1,dayOfMonth1,month1,year1);
  wait=wait+1;
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
  
  
  
if(dayOfWeek==6)
  {
   fralarm(hour,minute,second); 
  }
else if(dayOfWeek==7)
  {
   if(dayOfMonth>7&&dayOfMonth<15)
   {
    //second saturday
   } 
   else
   {
    alarm(hour,minute,second);

   }
  }
  else if(dayOfWeek==1)
  {
    //sunday
  }
  else
  {
  alarm(hour,minute,second);
  }

  lcd.clear();
  

   lcd.setCursor(count,1);
 
  String stringOne = "";
    stringOne=stringOne+"";
    stringOne=stringOne+String(dayOfMonth);
    stringOne=stringOne+"/";
    stringOne=stringOne+String(month);
    stringOne=stringOne+"/";
    stringOne=stringOne+String(year);
    stringOne=stringOne+" JJ bell system";
    stringOne.toCharArray(charBuf, 50) ;
  if(count<=0)
  {
  for(int j=k;j<strlen(charBuf);++j)
  {
    lcd.print(charBuf[j]); 
  }
   k=k+1;
    if(k>strlen(charBuf))
    {
      count=15;
      k=0;
    }
  }
  else
  {
   for(int j=0;j<strlen(charBuf);++j)
  {
    lcd.print(charBuf[j]);
    Serial.print(charBuf[j]);
  }
  count=count-1; 
  }
  
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
 lcd.setCursor(0,0);
 lcd.print("Time");
 lcd.print(hou);
 lcd.print(":");
  // convert the byte variable to a decimal number when displayed
  Serial.print(":");
  if (minute<10){
    Serial.print("0");
  }
  Serial.print(minute, DEC);
  lcd.print(minute,DEC);
  lcd.print(":");
  Serial.print(":");
  if (second<10){
    Serial.print("0");
  }
  Serial.print(second, DEC);
  lcd.print(second,DEC);
  lcd.print(" ");
  lcd.print(pre);
 
  }
}

void loop(){
  
  displayTime(); // display the real-time clock data on the Serial Monitor,
  delay(1000); // every second
}
