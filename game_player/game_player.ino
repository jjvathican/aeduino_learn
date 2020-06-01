/* Sweep
 by BARRAGAN <http://barraganstudio.com> 
 This example code is in the public domain.

 modified 28 May 2015
 by Michael C. Miller
 modified 8 Nov 2013
 by Scott Fitzgerald

 http://arduino.cc/en/Tutorial/Sweep
*/ 

#include <Servo.h> 

#include <Wire.h>
#include "RTClib.h"
int x =0;
int y=0;
int j=0; 
int s=0;
int m=0;
int t=0;
int k=0;
RTC_DS1307 RTC;
Servo myservo;  // create servo object to control a servo 
                // twelve servo objects can be created on most boards
 

void setup() 
{ 
pinMode(13,LOW);
Wire.begin();
RTC.begin();
RTC.adjust(DateTime(2014, 1, 21, 3, 0, 0));
myservo.attach(2);
Serial.begin(9600);
} 
 
void loop() 
{ 
 DateTime now = RTC.now();  
//1:
//   if(j%8==0)
//   { 
//  x=x+5;
//  Serial.print("ok");
//   }
//   x=x+1;
//  j=j+1;


//3:
j=now.second();
m=now.minute();
t=(m*60)+j;
//t=t/2;
y=map(t, 0, 80, 760 , 395);
y = constrain(y, 395, 760);
//if(y%8==0)
//{
//  y=y+1;
//}
Serial.println(j);
Serial.println(m);
Serial.println(t);
Serial.println(y);
Serial.println();
delay(y);
myservo.write(90); 
delay(100);
myservo.write(80);


//2:
//  delay(758-x);
//  Serial.print("1");
//   myservo.write(90); 
//   delay(100);
// myservo.write(80);
//  if(x%4==0)
// {
//  x=x+3;
// }
//
//   delay(395);
//  Serial.print("1");
//   myservo.write(90); 
//   delay(100);
// myservo.write(80);
//
// if(y%4==0)
// {
//  y=y+3;
// }
// 
//  x=x+1;
//  y=y+1;
//   delay(760);
//   Serial.print("2");
//   myservo.write(90); 
//   delay(100);
//  myservo.write(80);
//    delay(780);
//   myservo.write(90); 
//   delay(100);
//   myservo.write(80);
//    delay(790);
//   myservo.write(90); 
//   delay(100);
//   myservo.write(80);
} 

