#include <CapacitiveSensor.h>
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
void setup() {
    Serial.begin(9600);
    start = millis()+100;
    cs_4_6.set_CS_Timeout_Millis(2000);
    cs_4_6.reset_CS_AutoCal();
    pinMode(led,OUTPUT);
    pinMode(led2,OUTPUT);
}

void loop() {
    CSread();
}
long prev;
float alpha=0.01;
float average=0;
long cslimit=400;
long averagelimit=0;
void CSread() {
    long cs = cs_4_6.capacitiveSensor(100); //a: Sensor resolution is set to 80
   
       //check = millis()+50L;
   average = alpha * cs + (1-alpha) * average;    //exponential smoothing
   Serial.print("average:");
   Serial.println(average);   
   Serial.print("actual:");
   Serial.println(cs);   
       
   averagelimit= cslimit*0.8;   
  if(cs>cslimit && average > averagelimit*0.3)
  {
       digitalWrite(led2,HIGH);  
  }
  else
  {
      digitalWrite(led2,LOW); 
  }
  if (average > averagelimit && cs > cslimit) { //b: Arbitrary number
    csSum =csSum +(int(average)) + 0L;
    start = millis()+80L;
    //Serial.println(csSum);
  
    if (csSum >= 5000) //c: This value is the threshold, a High value means it takes longer to trigger
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
    
 //  digitalWrite(led2,LOW);         
    
   if(start<millis())
   {
    csSum = 0; //Timeout caused by bad readings
   }
  }
}
