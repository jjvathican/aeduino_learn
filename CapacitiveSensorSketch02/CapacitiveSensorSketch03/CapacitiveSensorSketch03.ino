#include <CapacitiveSensor.h>
#include <EEPROM.h>
/*
 * CapitiveSense Library Demo Sketch
 * Paul Badger 2008
 * Uses a high value resistor e.g. 10M between send pin and receive pin
 * Resistor effects sensitivity, experiment with values, 50K - 50M. Larger resistor values yield larger sensor values.
 * Receive pin is the sensor pin - try different amounts of foil/metal on this pin
 */

 long start = 0;
 long check=0;
CapacitiveSensor   cs_4_6 = CapacitiveSensor(4,6);        // 10M resistor between pins 4 & 6, pin 6 is sensor pin, add a wire and or foil
unsigned long csSum;
int led=13;
int led2=12;
long prev;
float alpha=0.01;
float average=0;
long cslimit=400;
long averagelimit=0;
void setup() {
    Serial.begin(9600);
    start = millis()+100;
    cs_4_6.set_CS_Timeout_Millis(2000);
    cs_4_6.reset_CS_AutoCal();
    pinMode(led,OUTPUT);
    pinMode(led2,OUTPUT);
    long maxval=EEPROMReadlong(0);
    long temp=300;
    for(int i=1;i<200;++i)
    {
      long cs = cs_4_6.capacitiveSensor(100);
      if(cs>temp && cs <500)
      {
        temp=cs;
        maxval=cs*0.5;
      }
      digitalWrite(led2,HIGH); 
      delay(10)  ;    
    }
     digitalWrite(led2,LOW); 
     delay(1000);
//     Serial.print("MAx VALUE IS:");
//     Serial.println(maxval);
       EEPROMWritelong(0,maxval);
       cslimit=maxval;
     
}

long EEPROMReadlong(long address) {
  long four = EEPROM.read(address);
  long three = EEPROM.read(address + 1);
  long two = EEPROM.read(address + 2);
  long one = EEPROM.read(address + 3);
  
  return ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);
}

void EEPROMWritelong(int address, long value) {
  byte four = (value & 0xFF);
  byte three = ((value >> 8) & 0xFF);
  byte two = ((value >> 16) & 0xFF);
  byte one = ((value >> 24) & 0xFF);
  
  EEPROM.write(address, four);
  EEPROM.write(address + 1, three);
  EEPROM.write(address + 2, two);
  EEPROM.write(address + 3, one);
}

void loop() {
    CSread();
}

void CSread() {
    long cs = cs_4_6.capacitiveSensor(100); //a: Sensor resolution is set to 80
   
       //check = millis()+50L;
   average = alpha * cs + (1-alpha) * average;    //exponential smoothing
   Serial.print("average:");
   Serial.println(average);   
   Serial.print("actual:");
   Serial.println(cs);   
       
   averagelimit= cslimit*0.8;   
  if(cs>cslimit && average > averagelimit*0.5)
  {
       digitalWrite(led2,HIGH);  
  }
  else
  {
     if(start<millis())
   {
      digitalWrite(led2,LOW); 
   }
  }
  if (average > averagelimit && cs > cslimit) { //b: Arbitrary number
    csSum =csSum +(int(average)) + 0L;
    start = millis()+50L;
    //Serial.println(csSum);
  
    if (csSum >= 8000) //c: This value is the threshold, a High value means it takes longer to trigger
    {
      
     // Serial.print("Trigger: ");
      //Serial.println(csSum);
      if (csSum > 0) { csSum = 0; } //Reset
      cs_4_6.reset_CS_AutoCal(); //Stops readings
      average=0;
      if(digitalRead(led)==0)
      {
        digitalWrite(led,HIGH);
      }
      else
      {
        digitalWrite(led,LOW);
      }
    }
  } else {
             
   if(start<millis())
   {
    csSum = 0; 
   }
  }
}
