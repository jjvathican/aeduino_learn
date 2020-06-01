#include <ADCTouch.h>

float ref0, ref1;     
int led=13;
float average=0;
float alpha=0.1;
float x=0;
float x1=0;
long y=0;
long y1=0;
float slope=0;
int state=0;

void resetfun()
{
  ref0 = ADCTouch.read(A0,100);   
  ref1 = ADCTouch.read(A1,100);  
}
void switchfun(float average)
{
    x=average;
    y=millis();
    if((x-x1)<0.01 and (x-x1)>-0.01)
    {
      slope=0;
    }
    else
    {
    slope=((y-y1)/(x-x1));
    }
    x1=x;
    y1=y;
    if(slope>500)
    {
    state=state+1;
    }
    else
    {
    if(state>=0)
    {  
    state=state-5; 
    } 
    }
    if(state>3)
    {
    if(digitalRead(led)==1)
    {
      digitalWrite(led ,LOW);
    }
    else
    {
       digitalWrite(led ,HIGH);
    }
    state=0;
    }
   

}
void setup() 
{
    Serial.begin(9600);
    resetfun();
} 

void loop() 
{
   pinMode(led,OUTPUT);
    long value0 = ADCTouch.read(A0);   
    long value1 = ADCTouch.read(A1);  
    
    value0 -= ref0;       
    value1 -= ref1;
    average = alpha * ((value0+value1)/2)+ (1-alpha) * average; 
    
   switchfun(average);
  
   Serial.print(value0);           
   Serial.print("\t");
	
   Serial.print(value1);
   Serial.print("\t");

   Serial.print(digitalRead(led));
   Serial.print("\t");
  
   Serial.print(average);
   Serial.print("\t");
   
   Serial.println(slope);
   
   delay(100);
}
