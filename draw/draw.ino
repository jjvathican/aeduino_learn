/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
*/

#include <Servo.h>

Servo servoleft; 
Servo servoright;
Servo servoup;
int pos = 0;    // variable to store the servo position

void setup() {
  servoleft.attach(7);
  servoright.attach(6);
  servoup.attach(9);
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
//  for (pos = 180; pos >= 90; pos -= 1) { // goes from 0 degrees to 180 degrees
//    // in steps of 1 degree
//    servoleft.write(pos);              // tell servo to go to position in variable 'pos'
//    delay(15);                       // waits 15ms for the servo to reach the position
//  }
//
//for (pos = 0; pos <= 90; pos += 1) { // goes from 0 degrees to 180 degrees
//    // in steps of 1 degree
//    servoright.write(pos);              // tell servo to go to position in variable 'pos'
//    delay(15);                       // waits 15ms for the servo to reach the position
//  }
//
//  
//  for (pos = 90; pos <= 180; pos += 1) { // goes from 180 degrees to 0 degrees
//    servoleft.write(pos);              // tell servo to go to position in variable 'pos'
//    delay(15);                       // waits 15ms for the servo to reach the position
//  }
//
//
//  for (pos = 90; pos >= 0; pos -= 1) { // goes from 0 degrees to 180 degrees
//    // in steps of 1 degree
//    servoright.write(pos);              // tell servo to go to position in variable 'pos'
//    delay(15);                       // waits 15ms for the servo to reach the position
//  }
//  servoleft.write(100);
 // servoright.write(90);
//  for (pos = 0; pos <= 100; pos += 1) { // goes from 180 degrees to 0 degrees
//    servoleft.write(175-pos);  
//    servoright.write(90-pos);  // tell servo to go to position in variable 'pos'
//    delay(15);                       // waits 15ms for the servo to reach the position
// }
/////////////////////////////////////////////
// for(int t=80;t<=180;t+=10)
// {
//  
//  for (pos = 0; pos <= (180-t); pos += 1) 
//  { 
//    servoleft.write(t+pos);   
//    int val= map(pos,0,(180-t),90,0); 
//    servoright.write(val);
//    delay(15);                       
//  }
//  for (pos = (180-t); pos >= 0; pos -= 1) 
//  { 
//    servoleft.write(t+pos);   
//    int val= map(pos,0,(180-t),90,0); 
//    servoright.write(val);
//    delay(15);                       
//  }
//  
// }
/////////////////////////////
  
//for(int t=90;t<=180;t+=10)
//{
//for (pos = 0; pos <= 80; pos += 1) { 
//    servoleft.write(t+pos);   
//    int val= map(pos,0,80,90,0); 
//    servoright.write(val);
//    delay(15);                       
// }
//}
//
//
//for(int t=90;t>=0;t-=10)
//{
//for (pos = 0; pos <= 80; pos += 1) { 
//    servoright.write(t-pos);   
//    int val= map(pos,0,80,90,180); 
//    servoleft.write(val);
//    delay(15);                       
// }
//}
//servoleft.write(90);

///////////////////////////////////////

//for(int j=80;j>=15;--j)
//{
//servoup.write(j);
//delay(20);
//}
//for(int j=15;j<=80;++j)
//{
//servoup.write(j);
//delay(20);
//}
//delay(500);
//int x=180;
//int y=90;
//int s=1;
//int d=10;
//int dv=46; 
//for(x=180;x>=150;x-=5)
//{
//
//for(int t=1;t<=90;t+=s)
//{
// if(t<=45)
// {
// digitalWrite(LED_BUILTIN, LOW); 
// servoright.write(y-t-((t%dv)/d));
// Serial.println(t%dv);
//servoleft.write(x-t+((t%dv)/d));
// }
// else
// {
//  digitalWrite(LED_BUILTIN, HIGH); 
//servoright.write(y-t-(((-t%dv)+dv)/d));
//Serial.println((-t%dv)+dv);
//servoleft.write(x-t+(((-t%dv)+dv)/d));
// }
//delay(50);
//}
//}
servoright.write(45);
for(int t=120;t>=90;t-=5)
{

for (pos = 10; pos <=30; pos += 1)
 { 
    servoleft.write(t+pos);   
    //int val= map(pos,10,40,90,0); 
    
    delay(100);                       
 }
}

//servoleft.write(120);
//Serial.print(servoleft.read());
//Serial.println(servoright.read());
//delay(200);



 
}

