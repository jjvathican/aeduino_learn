/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
*/

  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position
int count=0;
int val =0;
int del=50;
void setup() {
  Serial.begin(9600);
pinMode(8, OUTPUT);
pinMode(6, OUTPUT);// attaches the servo on pin 9 to the servo object
pinMode(7, OUTPUT);
pinMode(9,OUTPUT);
pinMode(2,INPUT);
}
void mcontrol(int i)
{
switch(i)
   {
    case 1://back
         digitalWrite(7, LOW);
         digitalWrite(8,LOW);
         digitalWrite(6, HIGH);
         digitalWrite(9,HIGH);
         
         break;
    case 2://forwrd
         digitalWrite(6, LOW);
         digitalWrite(9,LOW);
         digitalWrite(7, HIGH);
         digitalWrite(8,HIGH);
        
         break;  
    case 3://right
         digitalWrite(6, LOW);
         digitalWrite(8,LOW);
         digitalWrite(7, HIGH);
         digitalWrite(9,HIGH);
         
         break;   
    case 4://left
         digitalWrite(7, LOW);
         digitalWrite(9,LOW);
         digitalWrite(6, HIGH);
         digitalWrite(8,HIGH);
        
         break;      
     case 5://stop
         digitalWrite(7, LOW);
         digitalWrite(9,LOW);
         digitalWrite(6, LOW);
         digitalWrite(8,LOW);
        
         break;               
   }
}
 void goround(int vo)
 {
  if(vo==1)
  {
  count=0;
  while(digitalRead(2)==val&&count<8)
  {
    Serial.println("turning left");
  mcontrol(4);
  count=count+1;
  delay(del);
  
  }
  }
  if(vo==0)
  {
  count=0;
  while(digitalRead(2)==val&&count<15)
  {
  Serial.println("turning right");
  mcontrol(3);
  count=count+1;
  delay(del);
  }
  }
 }
 void forback(int vo)
 {
  if(vo==1)
  {
  count=0;
  while(digitalRead(2)==val&&count<5)
  {
  Serial.println("going backward");
  mcontrol(1);
  count=count+1;
  delay(del);
  }
  }
  if(vo==0)
  {
  count=0;
  while(digitalRead(2)==val&&count<5)
  {
  Serial.println("going forward");
  mcontrol(2);
  count=count+1;
  delay(del);
  }
  }
 }
 void fullround()
 {
  count=0;
  int set=0;
  int ncount=0;
  while(count<20)
  {
    count=count+1;
    mcontrol(4);
   delay(100);
   
     set=0;
     Serial.print("going round");
     Serial.print(ncount);
     Serial.println();
    while(digitalRead(2)!=val && count<20 )
    {
      set=1;
      mcontrol(4);
      delay(100);
      count=count+1;
      Serial.println("found black");
    }
    ncount=ncount+set;
   
  }
  if(ncount<1)
  {
    while(1)
    {
      mcontrol(5);
    }
  }
   if(ncount>=1)
    {
  int   m=0;
  mcontrol(5);
  delay(200);
  while(digitalRead(2)==val&&count<13)
  {
  Serial.println("turning right");
  mcontrol(3);
  m=m+1;
  delay(del);
  }
    }
 }
void loop() {
 Serial.println(digitalRead(2));
 if(digitalRead(2)!=val)
 {
   Serial.println("moving forward");
   mcontrol(2);
   delay(del);
 }

 else
 {
  
  Serial.println("stoopped");
  delay(del);
  mcontrol(5);
  delay(del);
  goround(1);
  delay(del);
  mcontrol(5);
 delay(del);
  forback(1);
  delay(del);
  mcontrol(5);
   delay(del);
  goround(0);
  delay(del);
  mcontrol(5);
 delay(del);
  forback(0);
  delay(del);
  mcontrol(5);
   delay(del);
  

   
 // fullround();
  delay(del);
  mcontrol(5);
 }
 // digitalWrite(7, HIGH);
 
}

