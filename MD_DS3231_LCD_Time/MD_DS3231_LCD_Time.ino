// Example code for the MD_DS3231 library
//
// Implements a clock display on a 16x2 LCD shield.
// Display updates are triggered by a 1 second a larm from the RTC alarm 1.
//
// The example shows using the library in polled mode, polled mode with callback, 
// or interrupt mode. Different modes of operation are enabled through setting 
// one of the USE_* defines at the top of the file.
//
#include <Metro.h>
#include <MD_DS3231.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <OneButton.h>
#include <Rotary.h>
#include <EEPROM.h>
Rotary r = Rotary(3, 2);
OneButton button(A0, true);
Metro ledMetro = Metro(150);
LiquidCrystal_I2C lcd(0x27,16,2);
int hour11=0;
int minute11=0;
int hour12=0;
int minute12=0;
int hour21=0;
int minute21=0;
int hour22=0;
int minute22=0;
int counter=0;
int menu=0;
int place =0;
int relay=12;
volatile boolean  bShowTime = false;

void p2dig(uint8_t v)
// print 2 digits with leading zero
{
  if (v < 10) lcd.print("0");
  lcd.print(v);
}

const char *dow2String(uint8_t code)
// Day of week to string. DOW 1=Sunday, 0 is undefined
{
  static const char *str[] = {"---", "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
  
  if (code > 7) code = 0;
  return(str[code]);
}
void motoron()
{
 Serial.println("MOTOR ON");
    digitalWrite(relay,HIGH);
    lcd.setCursor(10,1);
    lcd.print("ON "); 
}
void motoroff()
{
  Serial.println("MOTOR OFF");
    digitalWrite(relay,LOW);
    lcd.setCursor(10,1);
    lcd.print("OFF");
}
void motorems()
{
  Serial.println("MOTOR OFF");
    digitalWrite(relay,LOW);
    lcd.setCursor(10,1);
    lcd.print("EMS");
}
void checktime(int hour1,int minute1,int hour2,int minute2)
{
if(hour1<RTC.h && hour2>RTC.h)
  {
    motoron();
  }
  else if(hour1==RTC.h && hour2>RTC.h)
  {
    if(RTC.m>minute1)
    {
     motoron();
    }
    else
    {
    motoroff();    
    }
  }
   else if(hour1<RTC.h && hour2==RTC.h)
  {
    if(RTC.m<minute2)
    {
      motoron();
    }
    else
    {
      motoroff();
    }
  }
  else if(hour1==hour2)
  {
    if(minute1<=RTC.m && minute2>=RTC.m)
    {
    motoron();
    }
    else
    {
    motoroff();
    }
  }
  else
  {
    motoroff();
  }  
}
int emergency=1;
void printTime(void)
// Print the current time and date to the LCD display
{
  lcd.noBlink();
  lcd.setCursor(0,0);
  lcd.print(dow2String(RTC.dow));
  lcd.print(" ");  
  lcd.print(RTC.yyyy);
  lcd.print("-");
  p2dig(RTC.mm);
  lcd.print("-");
  p2dig(RTC.dd);
  
  lcd.setCursor(0,1);
  p2dig(RTC.h);
  lcd.print(":");
  p2dig(RTC.m);
  lcd.print(":");
  p2dig(RTC.s);
  
  
  
  //if((hour1*minute1)<(RTC.h*RTC.m) && (hour2*minute2)>(RTC.h*RTC.m)) 
  if(emergency==1)
  { 
  checktime(hour11,minute11,hour12,minute12);
  checktime(hour21,minute21,hour22,minute22);
  }
  else
  {
    motorems();
  }
}
void edittime(void)
{
  lcd.setCursor(0,0);
  lcd.print(dow2String(RTC.dow));
  lcd.print(" ");  
  lcd.print(RTC.yyyy);
  lcd.print("-");
  p2dig(RTC.mm);
  lcd.print("-");
  p2dig(RTC.dd);
  
  lcd.setCursor(0,1);
  p2dig(RTC.h);
  lcd.print(":");
  p2dig(RTC.m);
  lcd.print(":");
  p2dig(RTC.s);
  
  lcd.setCursor(10,1);
  lcd.print("<ed>");
  if(place==1)
  {
   RTC.h=counter%24; 
   lcd.setCursor(0,1);
   lcd.blink(); 
  }
  else if(place==2)
  {
   RTC.m=counter%60; 
   lcd.setCursor(3,1);
   lcd.blink(); 
  }
  else if(place==3)
  {
   RTC.dd=counter%31;
   lcd.setCursor(12,0);
   lcd.blink(); 
  }
  else if(place==4)
  {
    RTC.mm=counter%31;
   lcd.setCursor(9,0);
   lcd.blink(); 
  }
  else if(place==5)
  {
    RTC.yyyy=2015+counter;
   lcd.setCursor(4,0);
   lcd.blink();
  }
  else if(place==6)
  {
    RTC.dow=(counter%7)+1;
    lcd.setCursor(0,0);
    lcd.blink(); 
  }
  else if(place==7)
  {
    lcd.noBlink();
    lcd.clear();
    menu=0;
    place=0;
  }
  RTC.writeTime();
  
}
void displayUpdate(void)
{
  RTC.readTime();
  printTime();
}
void alarmICB(void)
{
  bShowTime = true;    // set the flag to update
}

void inputTime(void)
{
  RTC.yyyy = 2014;
  RTC.mm = 02;
  RTC.dd = 12;
  
  RTC.h = 15;
  RTC.m = 12;
  RTC.s = 12;
  
  RTC.dow = 03;
}  


void setup()
{
  hour11=EEPROM.read(0);
  minute11=EEPROM.read(1);
  hour12=EEPROM.read(2);
  minute12=EEPROM.read(3);
  hour21=EEPROM.read(4);
  minute21=EEPROM.read(5);
  hour22=EEPROM.read(6);
  minute22=EEPROM.read(7);
  pinMode(relay,OUTPUT);
  Serial.begin(9600);
  lcd.init();                      // initialize the lcd 
  lcd.init();
  lcd.backlight();
  lcd.noCursor();
   
PCICR |= (1 << PCIE2);
  PCMSK2 |= (1 << PCINT18) | (1 << PCINT19);
  sei();
  // click on the encoder
  button.attachDoubleClick(doubleclick);
  button.attachClick(singleclick);
  button.attachPress(longclick);

 RTC.control( DS3231_12H, DS3231_OFF);
 
RTC.setAlarm1Callback(alarmICB);
//RTC.setAlarm2Callback(alarmICB);

//pinMode(PIN_INTERRUPT, INPUT_PULLUP);
//attachInterrupt(digitalPinToInterrupt(PIN_INTERRUPT),turnoff, FALLING);
//RTC.control(DS3231_A2_INT_ENABLE, DS3231_ON); 
 
RTC.setAlarm1Type(DS3231_ALM_SEC);
//RTC.setAlarm2Type(DS3231_ALM_SEC);
//inputTime();
//RTC.writeTime();
}

void menu1()
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Enter ON/OFF T1"); 
  hour11=EEPROM.read(0);
  minute11=EEPROM.read(1);
  hour12=EEPROM.read(2);
  minute12=EEPROM.read(3);
}
void menu2()
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Enter ON/OFF T2"); 
  hour21=EEPROM.read(4);
  minute21=EEPROM.read(5);
  hour22=EEPROM.read(6);
  minute22=EEPROM.read(7);
}

void settime(int hour1,int minute1,int hour2,int minute2)
{
  lcd.setCursor(0,1);
  p2dig(hour1);
  lcd.print(":");
  p2dig(minute1);
  lcd.print("   ");
  p2dig(hour2);
  lcd.print(":");
  p2dig(minute2);
  if(place==1)
  {
   hour11=counter%24; 
   lcd.setCursor(0,1);
   lcd.blink(); 
  }
  else if(place==2)
  {
   minute11=counter%60; 
   lcd.setCursor(3,1);
   lcd.blink(); 
  }
  else if(place==3)
  {
    hour12=counter%24;
   lcd.setCursor(8,1);
   lcd.blink(); 
  }
  else if(place==4)
  {
    minute12=counter%60;
   lcd.setCursor(11,1);
   lcd.blink(); 
  }
  else if(place==5)
  {
     EEPROM.update(0,hour1);  
     EEPROM.update(1,minute1);
     EEPROM.update(2,hour2);
     EEPROM.update(3,minute2); 
     
    lcd.noBlink();
    lcd.clear(); 
    lcd.setCursor(0,0);
    unsigned long waitfun=millis()+1000;
    lcd.print("Saving...");
    while(waitfun>millis()); 
    menu=0;
    place=0;
  }
}

void loop()
{
RTC.checkAlarm1();
//RTC.checkAlarm2();
if (bShowTime) 
 {
  
  if(menu==0)
  {
    displayUpdate();
  }
 } 
if (ledMetro.check() == 1) 
  { 
 
  if(menu==1)
  {
    menu1();
    menu=menu+1;
  }
  else if(menu==2)
  {
    settime(hour11,minute11,hour12,minute12);
  }
  else if(menu==3)
  {
    menu2();
    menu=menu+1;
  }
  else if(menu==4)
  {
    settime(hour21,minute21,hour22,minute22);
  }
  else if(menu==5)
  {
    edittime();
  }
  else if(menu==6)
  {
    menu=0;
  }
  }
bShowTime = false;
button.tick();
}


void doubleclick() 
{
  Serial.println("DOUBLE");
  menu=menu+1;
  place=0;
  lcd.clear();
}
void singleclick() 
{
  Serial.println("SINGLE");
  counter=0;
  place=place+1;
   
}
void longclick() 
{
  Serial.println("LONG");
  emergency=emergency*-1;
}
ISR(PCINT2_vect)
{
  unsigned char result = r.process();
  if (result == DIR_NONE) {
    // do nothing
  }
  else if (result == DIR_CW) 
  {
    Serial.println("CLOCKWISE");
    counter=counter+1;
  }
  else if (result == DIR_CCW) 
  {
    Serial.println("COUNTERCLOCK");
    counter=counter-1;
  }
}
