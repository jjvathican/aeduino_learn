#include <CapacitiveSensor.h>

/*
 * CapitiveSense Library Demo Sketch
 * Paul Badger 2008
 * Uses a high value resistor e.g. 10M between send pin and receive pin
 * Resistor effects sensitivity, experiment with values, 50K - 50M. Larger resistor values yield larger sensor values.
 * Receive pin is the sensor pin - try different amounts of foil/metal on this pin
 */

 long start = 0;
CapacitiveSensor   cs_4_6 = CapacitiveSensor(4,6);        // 10M resistor between pins 4 & 6, pin 6 is sensor pin, add a wire and or foil
unsigned long csSum;
int led=13;
int led2=12;
void setup() {
    Serial.begin(9600);
    start = millis()+100;
    pinMode(led,OUTPUT);
    pinMode(led2,OUTPUT);
}

void loop() {
    CSread();
}

void CSread() {
    long cs = cs_4_6.capacitiveSensor(100); //a: Sensor resolution is set to 80
  if (cs > 350) { //b: Arbitrary number
    csSum += cs;
    start = millis()+300;
    Serial.println(cs);
    digitalWrite(led2,HIGH); 
    if (csSum >= 8800) //c: This value is the threshold, a High value means it takes longer to trigger
    {
      
      Serial.print("Trigger: ");
      if(digitalRead(led)==0)
      {
        digitalWrite(led,HIGH);
      }
      else
      {
        digitalWrite(led,LOW);
      }
      Serial.println(csSum);
      if (csSum > 0) { csSum = 0; } //Reset
      cs_4_6.reset_CS_AutoCal(); //Stops readings
    }
  } else {
    
           
    
   if(start<millis())
   {
    digitalWrite(led2,LOW);
    csSum = 0; //Timeout caused by bad readings
   }
  }
}
