#include "Wire.h"
#define DS3231_I2C_ADDRESS 0x68
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
#include "SD.h"
#define SD_ChipSelectPin 4
#include "TMRpcm.h"
#include "SPI.h"
#include <EEPROM.h>
TMRpcm tmrpcm;
int revbutton=2;
int choice=3;
int up=5;
int down=6;
int mode=7;
int relay=9;
int amplifier=10;
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
//char charBuf[50];
int starttime=538;
int fbell[12]={0,0,0,0,0,0,0,0,0,0,0,0};
int bell[12]={0,0,0,0,0,0,0,0,0,0,0,0};
// Convert normal decimal numbers to binary coded decimal
byte decToBcd(byte val){
  return( (val/10*16) + (val%10) );
}
// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val){
  return( (val/16*10) + (val%16) );
}
void setup(){
  digitalWrite(amplifier,LOW);
  tmrpcm.speakerPin = 8;
//Serial.begin(115200);
if (!SD.begin(SD_ChipSelectPin)) {
Serial.println("SD fail");
return;
}
  
  pinMode(revbutton,INPUT);
 
  pinMode(choice,INPUT);
  pinMode(up,INPUT);
  pinMode(down,INPUT);
  pinMode(mode,INPUT); 
  pinMode(relay,OUTPUT);
  pinMode(amplifier,OUTPUT);
  digitalWrite(amplifier,LOW);
  Wire.begin();
  Serial.begin(9600);
  lcd.begin();

  // Turn on the blacklight and print a message.
  lcd.backlight();
  lcd.print("Hello, world!");
  delay(5000);
  // set the initial time here:
  // DS3231 seconds, minutes, hours, day, date, month, year
  setDS3231time(50,57,8,2,17,6,19);
//  RTC.adjust(DateTime(__DATE__, __TIME__));

          int g;
          int x;
          for(g=0;g<12;g++)
          {
            bell[g]= EEPROM.read(g);
          }
          for(x=0;x<12;x++)
          {
           fbell[x]= EEPROM.read(g+x);
          }

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
    lcd.setCursor(0,1);
    lcd.print("(BELL !!)    "); 
    }
    else
    {
      digitalWrite(relay, LOW);
    }
  //   
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
  int totalminute=hour*60+minute;
    if(totalminute==starttime+fbell[1]+fbell[0]-5)
   {
   // digitalWrite(amplifier,HIGH);
    Serial.println("Amplifier_on");
   }
   if(totalminute==starttime+fbell[1]+fbell[0]+5)
   {     
    Serial.println("Amplifier_off");
    digitalWrite(amplifier,LOW); 
   }

    if(totalminute==starttime+fbell[11]+fbell[10]+fbell[9]+fbell[8]+fbell[7]+fbell[6]+fbell[5]+fbell[4]+fbell[3]+fbell[2]+fbell[1]+fbell[0]-5)
   {
  //  digitalWrite(amplifier,HIGH);
    Serial.println("Amplifier_on");
   }
   if(totalminute==starttime+fbell[11]+fbell[10]+fbell[9]+fbell[8]+fbell[7]+fbell[6]+fbell[5]+fbell[4]+fbell[3]+fbell[2]+fbell[1]+fbell[0]+5)
   {     
    Serial.println("Amplifier_off");
    digitalWrite(amplifier,LOW); 
   }
 
   if(totalminute==starttime+fbell[0])
   {
    playalarm(second,0,waitfor+2);
   }
   if(totalminute==starttime+fbell[1]+fbell[0])
   {
    playalarm(second,0,waitfor+2);
    if(second==5)
    { 
     music_play("shreya.wav");
     song_ending_alarm();
    }
   }
  
   if(totalminute==starttime+fbell[2]+fbell[1]+fbell[0])
   {
    playalarm(second,0,waitfor);
   }
   
   if(totalminute==starttime+fbell[3]+fbell[2]+fbell[1]+fbell[0])
   {
    playalarm(second,0,waitfor+1);
   }
 
   if(totalminute==starttime+fbell[4]+fbell[3]+fbell[2]+fbell[1]+fbell[0])
   {
    playalarm(second,0,waitfor+1);
   }
   if(totalminute==starttime+fbell[5]+fbell[4]+fbell[3]+fbell[2]+fbell[1]+fbell[0])
   {
    playalarm(second,0,waitfor);
   } 
   if(totalminute==starttime+fbell[6]+fbell[5]+fbell[4]+fbell[3]+fbell[2]+fbell[1]+fbell[0])
   {
    playalarm(second,0,waitfor+1);
   }
   if(totalminute==starttime+fbell[7]+fbell[6]+fbell[5]+fbell[4]+fbell[3]+fbell[2]+fbell[1]+fbell[0])
   {
    playalarm(second,0,waitfor+1);
   } 
   if(totalminute==starttime+fbell[8]+fbell[7]+fbell[6]+fbell[5]+fbell[4]+fbell[3]+fbell[2]+fbell[1]+fbell[0])
   {
    playalarm(second,0,waitfor);
   } 
   if(totalminute==starttime+fbell[9]+fbell[8]+fbell[7]+fbell[6]+fbell[5]+fbell[4]+fbell[3]+fbell[2]+fbell[1]+fbell[0])
   {
    playalarm(second,0,waitfor+1);
   }
   if(totalminute==starttime+fbell[10]+fbell[9]+fbell[8]+fbell[7]+fbell[6]+fbell[5]+fbell[4]+fbell[3]+fbell[2]+fbell[1]+fbell[0])
   {
    playalarm(second,0,waitfor+1);
   }
   if(totalminute==starttime+fbell[11]+fbell[10]+fbell[9]+fbell[8]+fbell[7]+fbell[6]+fbell[5]+fbell[4]+fbell[3]+fbell[2]+fbell[1]+fbell[0])
   {
    playalarm(second,0,1);
    if(second==5)
    { 
     music_play("jana.wav");
     song_ending_alarm();
    }
   }
}

void alarm(int hour,int minute,int second)
{
    int totalminute=hour*60+minute;
    if(totalminute==starttime+bell[1]+bell[0]-5)
   {
   // digitalWrite(amplifier,HIGH);
    Serial.println("Amplifier_on");
   }
   if(totalminute==starttime+bell[1]+bell[0]+5)
   {     
    Serial.println("Amplifier_off");
    digitalWrite(amplifier,LOW); 
   }

    if(totalminute==starttime+bell[11]+bell[10]+bell[9]+bell[8]+bell[7]+bell[6]+bell[5]+bell[4]+bell[3]+bell[2]+bell[1]+bell[0]-5)
   {
    //digitalWrite(amplifier,HIGH);
    Serial.println("Amplifier_on");
   }
   if(totalminute==starttime+bell[11]+bell[10]+bell[9]+bell[8]+bell[7]+bell[6]+bell[5]+bell[4]+bell[3]+bell[2]+bell[1]+bell[0]+5)
   {     
    Serial.println("Amplifier_off");
    digitalWrite(amplifier,LOW); 
   }
 
   if(totalminute==starttime+bell[0])
   {
    playalarm(second,0,waitfor+2);
   }
   if(totalminute==starttime+bell[1]+bell[0])
   {
    playalarm(second,0,waitfor+2);
    if(second==5)
    { 
     music_play("shreya.wav");
     song_ending_alarm();
    }
   }
  
   if(totalminute==starttime+bell[2]+bell[1]+bell[0])
   {
    playalarm(second,0,waitfor);
   }
   
   if(totalminute==starttime+bell[3]+bell[2]+bell[1]+bell[0])
   {
    playalarm(second,0,waitfor+1);
   }
 
   if(totalminute==starttime+bell[4]+bell[3]+bell[2]+bell[1]+bell[0])
   {
    playalarm(second,0,waitfor+1);
   }
   if(totalminute==starttime+bell[5]+bell[4]+bell[3]+bell[2]+bell[1]+bell[0])
   {
    playalarm(second,0,waitfor);
   } 
   if(totalminute==starttime+bell[6]+bell[5]+bell[4]+bell[3]+bell[2]+bell[1]+bell[0])
   {
    playalarm(second,0,waitfor+1);
   }
   if(totalminute==starttime+bell[7]+bell[6]+bell[5]+bell[4]+bell[3]+bell[2]+bell[1]+bell[0])
   {
    playalarm(second,0,waitfor+1);
   } 
   if(totalminute==starttime+bell[8]+bell[7]+bell[6]+bell[5]+bell[4]+bell[3]+bell[2]+bell[1]+bell[0])
   {
    playalarm(second,0,waitfor);
   } 
   if(totalminute==starttime+bell[9]+bell[8]+bell[7]+bell[6]+bell[5]+bell[4]+bell[3]+bell[2]+bell[1]+bell[0])
   {
    playalarm(second,0,waitfor+1);
   }
   if(totalminute==starttime+bell[10]+bell[9]+bell[8]+bell[7]+bell[6]+bell[5]+bell[4]+bell[3]+bell[2]+bell[1]+bell[0])
   {
    playalarm(second,0,waitfor+1);
   }
   if(totalminute==starttime+bell[11]+bell[10]+bell[9]+bell[8]+bell[7]+bell[6]+bell[5]+bell[4]+bell[3]+bell[2]+bell[1]+bell[0])
   {
    playalarm(second,0,1);
    if(second==5)
    { 
     music_play("jana.wav");
     song_ending_alarm();
    }
   }
}
 byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
void displayTime(){
  lcd.clear();
 
  // retrieve data from DS3231
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,
  &year);
  
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
//  String stringOne = "";  
//    stringOne=stringOne+"                ";
//    stringOne=stringOne+String(dayOfMonth);
//    stringOne=stringOne+"/";
//    stringOne=stringOne+String(month);
//    stringOne=stringOne+"/";
//    stringOne=stringOne+String(year);
//     stringOne=stringOne+"(";
//    switch(dayOfWeek){
//  case 1:
//    stringOne=stringOne+"Sunday";
//    break;
//  case 2:
//    stringOne=stringOne+"Monday";
//    break;
//  case 3:
//    stringOne=stringOne+"Tuesday";
//    break;
//  case 4:
//    stringOne=stringOne+"Wednesday";
//    break;
//  case 5:
//    stringOne=stringOne+"Thursday";
//    break;
//  case 6:
//    stringOne=stringOne+"Friday";
//    break;
//  case 7:
//    stringOne=stringOne+"Saturday";
//    break;
//  }
//   stringOne=stringOne+")";
//  stringOne=stringOne+" JJ Bell System ";
//    stringOne=stringOne+"#Contact:9544414811  ";
//  stringOne=stringOne+"                 ";
//    stringOne.toCharArray(charBuf, 100) ;
//  lcd.setCursor(count,1);
//  
//   for(int j=k;j<=15+k;++j)
//  {
//    lcd.print(charBuf[j]);
//   // Serial.print(charBuf[j]);
//  }
//  k=k+1;
//  //Serial.println();
//    if(k+16>strlen(charBuf))
//    {
//      k=0;
//    }
//     
}
int km=1;
int cc=1;
void TimeAdjust()
{
  lcd.clear();
  lcd.setCursor(0,0);
   Serial.println(digitalRead(choice));
  if(digitalRead(choice)==1)
  {
    while(digitalRead(choice)==1){}
    //lcd.print(km);
    km=km+1;
  }
  if(digitalRead(revbutton)==1)
  {
    while(digitalRead(revbutton)==1){}
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
          change_time(&second1,60);
          break;
      case 2:
          lcd.print("minute :");
          lcd.print(minute1);
          change_time(&minute1,60);
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
          lcd.print("bell ");
          lcd.print(cc);
          change_time(&cc,12);
          break;
      case 9:
          lcd.print("duration ");
          lcd.print(bell[cc]);
          change_time(&bell[cc],60);
          break;
      case 10:
          lcd.print("fbell ");
          lcd.print(cc);
          change_time(&cc,12); 
          break; 
      case 11:
          lcd.print("duration ");
          lcd.print(fbell[cc]);
          change_time(&fbell[cc],60);  
          break;  
      case 13:
          setDS3231time(second1,minute1,hour1,dayOfWeek1,dayOfMonth1,month1,year1);
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("saved...!");
          delay(1000);
          km=1;
          break;    
      case 12:
          int g;
          int x;
          for(g=0;g<12;g++)
          {
            EEPROM.write(g,bell[g]);
          }
          for(x=0;x<12;x++)
          {
            EEPROM.write(g+x,fbell[x]);
          }  
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("saved bell...!");
          break;             
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
           if(*mm<0)
           {
           *mm=*mm+tt;
           }  
          }
}
void loop(){
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth,&month,&year);
  second1=second; 
  minute1=minute;
  hour1=hour;
  dayOfWeek1=dayOfWeek;
  dayOfMonth1=dayOfMonth;
  month1=month;
  year1=year;
  Serial.println(digitalRead(mode));
  if(digitalRead(mode)==1)
  {   
    displayTime();
    km=1;
    delay(1000); 
  }
  else
  {
     //Serial.println(second1);
     while(digitalRead(mode)!=1)
      {
       TimeAdjust();
       delay(100);
      }
   
  } 
}
