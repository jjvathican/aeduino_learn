#include <ADCTouch.h>

float ref0, ref1,ref2,ref3,ref4,ref5,ref6,ref7;
int led = 3;
int aled = 2;
float average = 0;
float filter = 0;
float alpha = 0.1;
float x = 0;
float x1 = 0;
long y = 0;
long y1 = 0;
float slope = 0;
long state = 0;
long prev_slope;
long slope_count = 0;

void resetfun()
{
  ref0 = ADCTouch.read(A0, 2000);
  ref1 = ADCTouch.read(A1, 2000);
  ref2 = ADCTouch.read(A2, 2000);
  ref3 = ADCTouch.read(A3, 2000);
  ref4 = ADCTouch.read(A4, 2000);
  ref5 = ADCTouch.read(A5, 2000);
  ref6 = ADCTouch.read(A6, 2000);
  ref7 = ADCTouch.read(A7, 2000);
}
void switchfun(float average)
{

  x = average;
  y = millis();
  if ((x - x1) < 0.01 and (x - x1) > -0.01)
  {
    slope = 0;
  }
  else
  {
    slope = ((y - y1) / (x - x1));
  }
  x1 = x;
  y1 = y;

  if (slope==0)
  {
     if ((state / slope_count) > 10)
    {
//      if (digitalRead(led) == 1)
//      {
//        digitalWrite(led , LOW);
//      }
//      else
      {
        digitalWrite(led , HIGH);
      }
     
   // delay(5000);
    }
     state = 0;
      slope_count = 0;
  }
  else if (slope > prev_slope)
  {
    prev_slope = slope;
    slope_count = slope_count + 1;
    state = state + x;
    digitalWrite(aled , HIGH);
  }
  else if (slope < prev_slope)
  {
    prev_slope = slope;
    slope_count = slope_count + 1;
    state = state + x;
    digitalWrite(aled , LOW);
  }
  //    else if(slope<0)
  //    {
  //      // digitalWrite(aled ,LOW);
  //
  //    }



}
void setup()
{
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  pinMode(aled, OUTPUT);
  resetfun();
  digitalWrite(aled , HIGH);
}

void loop()
{
  pinMode(led, OUTPUT);
  long value0 = ADCTouch.read(A0);
  long value1 = ADCTouch.read(A1);
  long value2 = ADCTouch.read(A2);
  long value3 = ADCTouch.read(A3);
  long value4 = ADCTouch.read(A4);
  long value5 = ADCTouch.read(A5);
  long value6 = ADCTouch.read(A6);
  long value7 = ADCTouch.read(A7);
  value0 -= ref0;
  value1 -= ref1;
  value2 -= ref2;
  value3 -= ref3;
  value4 -= ref4;
  value5 -= ref5;
  value6 -= ref6;
  value7 -= ref7;
//  if((value1-value0)<2 && (value1-value0)>0 || (value1-value0)<2 && (value1-value0)>0)
//  {
  average = (value0 + value1+ value2+ value3+ value4+ value5+ value6+ value7) / 8;
//  filter = alpha * ((value0 + value1+ value2+ value3+ value4+ value5+ value6+ value7) / 8) + (1 - alpha) * filter;
//  switchfun(filter);
 // }
if(average>70)
{
  digitalWrite(led , HIGH); 
}

//  Serial.print(value0);
//  Serial.print("\t");
//
//  Serial.print(value1);
//  Serial.print("\t");
//
//  Serial.print( slope_count);
//  Serial.print("\t");


  Serial.print(average);
  Serial.print("\t");
  //
  Serial.print((state / slope_count));
  Serial.print("\t");
  //
  //
  //
  Serial.println(slope);
  //
//  delay(100);

}
