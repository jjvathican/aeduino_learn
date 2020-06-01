#include "Wire.h"
#define DS3231_I2C_ADDRESS 0x68
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
#include "SD.h"
#define SD_ChipSelectPin 53
#include "TMRpcm.h"
#include "SPI.h"

TMRpcm tmrpcm;
int choice=6;
int up=5;
int down=4;
int mode=7;
int relay=10;
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
int timecount=0;
int amplifierpin=39;
int timearray[100][5];
 
char charBuf[100];
// Convert normal decimal numbers to binary coded decimal
byte decToBcd(byte val){
  return( (val/10*16) + (val%10) );
}
// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val){
  return( (val/16*10) + (val%16) );
}
void setup(){
tmrpcm.speakerPin = 46;
if (!SD.begin(SD_ChipSelectPin)) {
Serial.println("SD fail");
return;
}
  pinMode(amplifierpin,OUTPUT);
  pinMode(choice,INPUT);
  pinMode(up,INPUT);
  pinMode(down,INPUT);
  pinMode(mode,INPUT); 
  pinMode(relay,OUTPUT);
  digitalWrite(relay,LOW);
  Wire.begin();
  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();
  setDS3231time(55,58,8,2,17,6,19);

int i=0;
char charmem[10];

  File dataFile = SD.open("datalog.txt");
  if (dataFile) {
    while (dataFile.available()) 
    {
        String s = dataFile.readStringUntil(';');
        s.trim();
        s.toCharArray(charmem, 75) ;

        char* command = strtok(charmem,":");
        int kk=0;
        while (command != 0)
        {
        // Serial.println(atoi(command));
          timearray[i][kk]=atoi(command);
         Serial.println(timearray[i][kk]);
          command = strtok(0, ":");
          kk=kk+1;
        }

         
        i=i+1;
    }
    dataFile.close();
   
  
    
  }
  else {
    Serial.println("error opening datalog.txt");
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
void playalarm(int secondtt,int increment,int waittime)
{
  if(secondtt>=increment&& secondtt<=(waittime+increment))
  {
  digitalWrite(relay, HIGH);
  lcd.setCursor(0,0);
  lcd.print("(BELL !!)      "); 
  }
  else
  {
    digitalWrite(relay, LOW);
  }
   
}
void music_play(int songindex)
{     
   if(songindex!=0)
   {
      String abc="";
      switch(songindex)
      {
       case 1: abc= "shreya.wav";break;
       case 2: abc= "jana.wav";break;
      }
      char songname[20];
      abc.toCharArray(songname,20) ;
      tmrpcm.play(songname);
      while(tmrpcm.isPlaying())
      {
         lcd.setCursor(0,0);
         lcd.print("(PLAYING !!)    "); 
         delay(200);
      }
      delay(1000);
      song_ending_alarm();
   }
     
}
void song_ending_alarm()
{
     lcd.setCursor(0,0);
     lcd.print("(BELL !!)      ");  
     delay(500);
     digitalWrite(relay, HIGH);
     delay(2000);
     digitalWrite(relay, LOW);
}

void amplifier_on(int hourtt,int minutett,int arrayin[][5])
{
 
  if(arrayin[timecount][4]>0)
  {
  int totalminute=arrayin[timecount][0]*60+arrayin[timecount][1];
  int relayon=totalminute-10;
  int relayoff=totalminute+10;
  int currenttime=hourtt*60+minutett;
  Serial.print(relayon);
  Serial.print(" ");
  Serial.print(relayoff);
  Serial.print(" ");
  Serial.println(currenttime);
  if(relayon>currenttime && currenttime<relayoff)
  {
    if(currenttime==relayon)
    {
    lcd.setCursor(0,0);
    lcd.print("WARMING STARTED "); 
    delay(2000);
    }
    digitalWrite(amplifierpin,HIGH);
  }
  else
  {
    if(currenttime==relayoff)
    {
    lcd.setCursor(0,0);
    lcd.print("Amplifier off ");
    delay(2000); 
    }
    digitalWrite(amplifierpin,LOW);
  }
  }
  
}
void select_time(int hourtt,int minutett,int arrayin[][5])
{
  int sizeis = (sizeof(timearray)/sizeof(int))/5;
  Serial.print("sizeis:");
  Serial.println(sizeis); 
  if(timecount<sizeis && timecount >=0)
  {
  if(arrayin[timecount][0]<hourtt)
  {
    timecount=timecount+1;
  }
  else if(arrayin[timecount][0]==hourtt)
  {
    if(arrayin[timecount][1]< minutett)
    {
       timecount=timecount+1;
    }
  }
  }
  else
  {
   timecount=-1; 
  }
}
void alarm(int hourtt,int minutett,int secondtt,int arrayin[][5])
{
 
 select_time(hourtt,minutett,arrayin);
 amplifier_on(hourtt,minutett,arrayin);
 if(hourtt==arrayin[timecount][0]) 
 { 
 Serial.println("hour ok");
   if(minutett==arrayin[timecount][1])
   {
    Serial.println("minute ok");
    playalarm(secondtt,arrayin[timecount][2],arrayin[timecount][3]);
    if(secondtt==5+arrayin[timecount][2])
    { 
     music_play(arrayin[timecount][4]);
     Serial.println("hello");
    } 
   }
   
 } 
}
 
void displayTime(int hourtt,int minutett,int secondtt,int dayOfWeektt,int dayOfMonthtt,int monthtt,int yeartt)
{
  lcd.clear();
 
  String pre="";
  if(hourtt<12)
  {
    pre="AM";
  }
  else
  {
    pre="PM";
  }
  if(int(secondtt/5)%2==0)
  {
  int hou;
  if(hourtt%12==0)
  {
    hou=12;
  }
  else
  {
    hou=hourtt%12;
  }
   lcd.print("TIME ");
 if (hou<10) lcd.print('0');
 lcd.print(hou);
 lcd.print(":");
  if (minutett<10) lcd.print('0');
  lcd.print(minutett,DEC);
//  lcd.print(":");
//  if (secondtt<10) lcd.print('0');
//  lcd.print(secondtt,DEC);
  lcd.print(" ");
  lcd.print(pre);
  }
  else
  {
    if(timecount==-1)
    {
     lcd.print("GOOD EVENING"); 
    }
    else
    {
    int hou;
    if(timearray[timecount][0]%12==0)
    {
    hou=12;
    }
    else
    {
    hou=timearray[timecount][0]%12;
    }
    lcd.print("NEXT ");
    if (hou<10) lcd.print('0');
    lcd.print(hou);
    lcd.print(":");
    if (timearray[timecount][1]<10) lcd.print('0');
    lcd.print(timearray[timecount][1]);
    lcd.print(" ");
    lcd.print(pre);
    }
  }
if(timecount!=-1)
{  
if(dayOfWeektt==6)//friday
  {
  // fralarm(hour,minute,second); 
  }
else if(dayOfWeektt==7)//saturday
  {
   if(dayOfMonthtt>7&&dayOfMonthtt<15)
   {
    
   } 
   else
   {
    alarm(hourtt,minutett,secondtt,timearray);
   }
  }
else if(dayOfWeektt==1)//sunday
{
  
}
else
  {
  alarm(hourtt,minutett,secondtt,timearray);
  }
}
  
  String stringOne = "";  
    stringOne=stringOne+"                ";
    stringOne=stringOne+String(dayOfMonthtt);
    stringOne=stringOne+"/";
    stringOne=stringOne+String(monthtt);
    stringOne=stringOne+"/";
    stringOne=stringOne+String(yeartt);
     stringOne=stringOne+"(";
    switch(dayOfWeektt){
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
//charBuf[100]={'a','b','c','d'};
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
     
}
int km=1;
void TimeAdjust(int hourtt,int minutett,int secondtt,int dayOfWeektt,int dayOfMonthtt,int monthtt,int yeartt)
{
  while(digitalRead(mode)!=1)
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
          lcd.print("second :");
          lcd.print(secondtt);
          change_time(&secondtt,60);
          break;
      case 2:
          lcd.print("minute :");
          lcd.print(minutett);
          change_time(&minutett,60);
          break;
      case 3:
          lcd.print("hour :");
          lcd.print(hourtt);
          change_time(&hourtt,24); 
          break; 
      case 4:
          lcd.print("dayOfWeek :");
          lcd.print(dayOfWeektt);
          change_time(&dayOfWeektt,7); 
          break;    
      case 5:
          lcd.print("dayOfMonth :");
          lcd.print(dayOfMonthtt);
          change_time(&dayOfMonthtt,31); 
          break;           
      case 6:
          lcd.print("month :");
          lcd.print(monthtt);
          change_time(&monthtt,12); 
          break;
      case 7:
          lcd.print("year :");
          lcd.print(yeartt);
          change_time(&yeartt,25); 
          break; 
      
      case 8:
          lcd.print("saving.....");
          setDS3231time(secondtt,minutett,hourtt,dayOfWeektt,dayOfMonthtt,monthtt,yeartt);
          delay(1000);
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("saved...!");
          delay(1000);
          km=1;
          break;               
    }
    delay(150);
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





int nextweekday = 1;
void loop(){



  
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,
  &year);
  if(nextweekday==dayOfWeek)
  {
    nextweekday=(dayOfWeek%7)+1;
    timecount=0;
  }
  if(digitalRead(mode)==1)
  {   
    displayTime(hour, minute, second, dayOfWeek, dayOfMonth, month, year);
    km=1;
    delay(1000); 
  }
  else
  {
     //Serial.println(second1);
     
       TimeAdjust(hour, minute,second, dayOfWeek, dayOfMonth, month, year);
       delay(100);
      
   
  } 
}
