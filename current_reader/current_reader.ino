
 
/*
 * 
 * Arduino Sketch for Allegro ACS712 Current Sensor 
 * this sensor can measure current at range of up to 30A
 * It operates with 5V
 * Please watch video instruction and explanation for this code.
 * 
 * Written by Ahmad Shamshiri on Sunday Jun 17, 2018 at 22:06 at Ajax, Ontario, Canada
 * for Robojax.com
 * View the video instruction at https://youtu.be/DVp9k3xu9IQ
 * This code has been downloaded from Robojax.com
 */




#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
//#include <EEPROM.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);
#define VIN A0 // define the Arduino pin A0 as voltage input (V in)
const float VCC   = 5.0;// supply voltage is from 4.5 to 5.5V. Normally 5V.
const int model = 2;   // enter the model number (see below)
int resetpin=3;


float cutOffLimit = 0.3;// set the current which below that value, doesn't matter. Or set 0.5

/*
          "ACS712ELCTR-05B-T",// for model use 0
          "ACS712ELCTR-20A-T",// for model use 1
          "ACS712ELCTR-30A-T"// for model use 2  
sensitivity array is holding the sensitivy of the  ACS712
current sensors. Do not change. All values are from page 5  of data sheet          
*/
float sensitivity[] ={
          0.185,// for ACS712ELCTR-05B-T
          0.100,// for ACS712ELCTR-20A-T
          0.066// for ACS712ELCTR-30A-T
     
         }; 


const float QOV =   0.5 * VCC;// set quiescent Output voltage of 0.5V
float voltage;// internal variable for voltage
float current;
float prev=0;
float raw=0;
int average=0;
long unsigned currentmilli;
long unsigned delaymilli;
long unsigned setmilli;
float correction=-0.01;
int count=0;
void setup() 
{
  pinMode(resetpin,INPUT);
//   
//    currentmilli = EEPROM.read(1);
//    delaymilli=EEPROM.read(2);
//    setmilli=EEPROM.read(3);
    Serial.begin(9600);// initialize serial monitor
    lcd.begin();
    
  // Turn on the blacklight and print a message.
  lcd.backlight();
}

void loop() {
 
 
Serial.print("reset value:");
Serial.println(digitalRead(resetpin)); 
//if(digitalRead(resetpin)==1)
//{
//    EEPROM.write(1, 0);
//    EEPROM.write(2, 0);
//    EEPROM.write(3, 0);
//}
//Serial.println(EEPROM.read(1));
//    Serial.println(EEPROM.read(2));

float maxread=0;
float minread=0;

float val=0;

// 
minread= analogRead(VIN);
for(int j=0;j<=140;++j)
{
  raw =  analogRead(VIN);
   float  voltage_raw =   (5.0 / 1023.0)*  raw;
   voltage =  voltage_raw - QOV  ;
  if(maxread<voltage)
  {
   maxread=voltage; 
  }
  if(minread>voltage)
  {
    minread=voltage;
  }
  delay(10);
}
//raw =analogRead(VIN);
 float setit=((maxread+abs(minread))*.4)+correction;
  Serial.println(setit,3);
   if(count>100)
  {
    count=0;
  }
   count=count+1;
    Serial.println(count);
  average= (average + int(setit*1000)%10);
   Serial.println(average);
  float increm=float((int(average/count))/10);
  Serial.println(increm,3);
 //Serial.println(increm/1000),3);
 setit=(setit-float(int(setit*1000)%10)/1000)+float(increm/1000);
   
  Serial.println(setit,3);
 Serial.println(prev,3);

 
 current =  setit  / sensitivity[model];


 if(current>=0.00 ||current<0.00 )
 {
    Serial.print("max: ");
    Serial.print(maxread,3);// print voltage with 3 decimal places
    Serial.print(", mix: ");
    Serial.print(minread,3);
     Serial.print("V, V: ");
    Serial.print(voltage,3);
    Serial.print("V, I: ");
    Serial.print(current,3); // print the current with 2 decimal places
    Serial.print("A, W: ");
    Serial.println(current*230,3); // print the current with 2 decimal places

    
    lcd.setCursor(0,0);
    lcd.print("V:");
    lcd.print(voltage,3);
    lcd.setCursor(9,0);
    lcd.print("I:");
    lcd.print(current,3);
    lcd.setCursor(0,1);
    lcd.print("W:");
    lcd.print((current*220),0);
    lcd.setCursor(7,1);
    
    currentmilli=millis()-delaymilli;
//    EEPROM.write(1, currentmilli);
//    EEPROM.write(3, millis());
    float kwh=((current*220)/1000) * ((currentmilli/1000));
    if(((currentmilli/1000)/10)%2==0)
    {
    
    lcd.print("WH:");
    lcd.print(kwh,4);
    }
    else
    {
     lcd.print("Rs:");
     float rs=kwh*6;
      lcd.print(rs,4); 
    }
 }
 else
 {
   delaymilli=millis()-currentmilli;
 //  EEPROM.write(2, delaymilli);
    lcd.print("low current");
 }

// delay(2500);
} 
  

