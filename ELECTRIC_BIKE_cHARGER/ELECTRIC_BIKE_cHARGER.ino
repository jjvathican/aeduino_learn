
#include <OneButton.h>
#include <Rotary.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>
#include <Metro.h>
Metro motorMetro = Metro(65000);
//Metro clockMetro = Metro(1000);
unsigned long clocktime=0;

Metro menuMetro=Metro(500);
Rotary r = Rotary(3, 2);
OneButton button(A0, true);
int relay1=10;
int speaker  =7;
LiquidCrystal_I2C lcd(0x27,16,2);

uint8_t hourA=0;
uint8_t minuteA=0;
uint8_t hourB=0;
uint8_t minuteB=0;

uint8_t hour1=0;
uint8_t minute2=0;
uint8_t hour3=0;
uint8_t minute4=0;

void p2dig(uint8_t v)
{
  if (v < 10) lcd.print("0");
  lcd.print(v);
}


void setup()
{
  hourA=EEPROM.read(0);
  minuteA=EEPROM.read(1);
  hourB=EEPROM.read(2);
  minuteB=EEPROM.read(3);

      hour1=EEPROM.read(4);
     minute2=EEPROM.read(5);
     hour3=EEPROM.read(6);
     minute4=EEPROM.read(7); 

  lcd.init();
  lcd.backlight();
  lcd.noCursor();
  pinMode(relay1,OUTPUT);
  pinMode(speaker,OUTPUT);
  Serial.begin(115200);
  //rotory interrupt
  PCICR |= (1 << PCIE2);
  PCMSK2 |= (1 << PCINT18) | (1 << PCINT19);
  sei();
  // click on the encoder
  button.attachDoubleClick(doubleclick);
  button.attachClick(singleclick);
  button.attachPress(longclick);
   clocktime=millis()+1000;
}
uint8_t place=0;
uint8_t menu=0;
uint8_t pause_state=-1;
int counter=0;
void loop()
{
 
  button.tick();
 
  
  if (menuMetro.check() == 1) 
   {
  
  
  if(menu==1)
  {
   lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("START/STOP"); 
  lcd.setCursor(0,1);
  p2dig(hour1);
  lcd.print(":");
  p2dig(minute2);
  lcd.print("   ");
  p2dig(hour3);
  lcd.print(":");
  p2dig(minute4);   
   lcd.blink();
   placeselect();
   pause_state=1;
  }
  else if(menu==0)
  {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("START/STOP"); 
  lcd.setCursor(0,1);
  p2dig(hourA);
  lcd.print(":");
  p2dig(minuteA);
  lcd.print("   ");
  p2dig(hourB);
  lcd.print(":");
  p2dig(minuteB);   
  lcd.noBlink();
  if(pause_state==1)
  {  
    motorpau();
  }
  else
  {
    decrement()  ;
   
  }
  }
   }
  
} 
long clckfunc(uint8_t h,uint8_t m)
{
  if((m+(h*60))<=0)
  {
    return -1;
  }
  if (clocktime<millis()) 
  {
   clocktime=millis()+1000;
   return (m+(h*60))-1;
  }
  else
  {
    return (m+(h*60));
  }
}

void decrement()
{

   
  long abc=clckfunc(hourA,minuteA);//((minuteA+(hourA*60)))-1;

  if(abc>=0)
  {
  //digitalWrite(relay1,0);
  motoroff();

  hourA=abc/60;
  minuteA=abc%60;
  }
  else
  {
     
     long xyz=clckfunc(hourB,minuteB);
     if(xyz>=0)
     {
     motoron();
     digitalWrite(relay1,1);
     hourB=xyz/60;
     minuteB=xyz%60;
     }
     else
     {
     motoroff();
     digitalWrite(relay1,0);
     }
     
  }
  if (motorMetro.check() == 1) 
   {
   EEPROM.update(0,hourA);  
   EEPROM.update(1,minuteA);
   EEPROM.update(2,hourB);
   EEPROM.update(3,minuteB);
   }
}

void placeselect()
{
 if(place==0)
 {
   lcd.setCursor(1,1); 
   if((hour1+counter)>=0)
   {
   hour1=(hour1+counter)%24;//(hourA+counter)%24;
   }
   counter=0; 
 }
 else if(place==1)
 {
   lcd.setCursor(4,1);
   if((minute2+counter)>=0)
   {
   minute2=(minute2+counter)%60;
   } 
   counter=0;
 }
 else if(place==2)
 {
   lcd.setCursor(9,1);
    if((hour3+counter)>=0)
   {
   hour3=(hour3+counter)%24;
   }
   counter=0; 
 }
  else if(place==3)
 {
   lcd.setCursor(12,1);
   if((minute4+counter)>=0)
   {
   minute4=(minute4+counter)%60;
   }
   counter=0; 
 }
 else
 {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("SAVING..."); 
  
     hourA=EEPROM.read(4);
     minuteA=EEPROM.read(5);
     hourB=EEPROM.read(6);
     minuteB=EEPROM.read(7);
     EEPROM.update(0,hourA);  
   EEPROM.update(1,minuteA);
   EEPROM.update(2,hourB);
   EEPROM.update(3,minuteB);
     menu=0;
 }
  
   EEPROM.update(4,hour1);  
   EEPROM.update(5,minute2);
   EEPROM.update(6,hour3);
   EEPROM.update(7,minute4);
}
void doubleclick() 
{
tone(speaker,5000, 50);
place=0;
menu=(menu+1)%2;
}
void singleclick() 
{
tone(speaker,5000, 50);  
counter=0;
place =(place+1)%5;
}
void longclick()
{
  tone(speaker,5000, 50);
  Serial.println("LONG");
  pause_state=pause_state*-1;
}
ISR(PCINT2_vect)
{
  unsigned char result = r.process();
  if (result == DIR_NONE) {
    // do nothing
  }
  else if (result == DIR_CW) {

    Serial.println("CLOCKWISE");
    tone(speaker,5000, 50);
    counter=counter-1;
  }
  else if (result == DIR_CCW) 
  {
    Serial.println("COUNTERCLOCK");
    tone(speaker,5000, 50);
    counter=counter+1;
  }
}
int state=0;
void motoron()
{
  if(state==1)
  {
    tone(speaker,5000, 1000);
    state=0;
  }
 Serial.println("MOTOR ON");
    digitalWrite(relay1,HIGH);
    lcd.setCursor(12,0);
    lcd.print("ON "); 
}

void motoroff()
{
  Serial.println("MOTOR OFF");
  if(state==0)
  {
    tone(speaker,5000, 1000);
    state=1;
  }
    digitalWrite(relay1,LOW);
    lcd.setCursor(12,0);
    lcd.print("OFF");
}
void motorpau()
{
  Serial.println("MOTOR PAU");
    digitalWrite(relay1,LOW);
    lcd.setCursor(12,0);
    lcd.print("PAU");
}
