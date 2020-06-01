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
long state=0;
long prev_slope;
long cou=10;

void resetfun()
{
  ref0 = ADCTouch.read(A0,2000);   
  ref1 = ADCTouch.read(A1,2000);  
}
void switchfun(float average)
{
   
    x=average;
    y=millis();
    if((x-x1)<0.1 and (x-x1)>-0.1)
    {
      slope=0;
    }
    else
    {
    slope=((y-y1)/(x-x1));
    }
    x1=x;
    y1=y;
    
    if(slope>prev_slope && slope>100)
    {
      prev_slope=slope+50;
      state=state+(int)slope;   
    }
    else if(slope<0)
    {
        if(state>3000)
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
    else
    {
    prev_slope=slope;
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
