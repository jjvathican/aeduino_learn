/*
  Arrays

  Demonstrates the use of an array to hold pin numbers in order to iterate over
  the pins in a sequence. Lights multiple LEDs in sequence, then in reverse.

  Unlike the For Loop tutorial, where the pins have to be contiguous, here the
  pins can be in any random order.

  The circuit:
  - LEDs from pins 2 through 7 to ground

  created 2006
  by David A. Mellis
  modified 30 Aug 2011
  by Tom Igoe

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/Array
*/

int timer = 200;
int but[]={
  12,11,10,9,8
};
int ledPins[] = {
  6,5,4,3,2
};
int state=255;
int limit=100;
int memory[100][5];  
int butCount=5;// an array of pin numbers to which LEDs are attached
int pinCount =5 ;
int mod=5;// the number of pins (i.e. the length of the array)
const int swch = 7;
const int lrn=13;
int count=0;
void setup() {
  // the array elements are numbered from 0 to (pinCount - 1).
  // use a for loop to initialize each pin as an output:
   for(int j=0;j<limit;++j)
  {
   for(int i=0;i<pinCount;++i)
  {
     memory[j][i]=0; 
  }
  }
    Serial.begin(9600);
    pinMode(swch,INPUT);
    pinMode(lrn,INPUT);
    for (int thisPin = 0; thisPin < butCount; thisPin++) {
        pinMode(but[thisPin],INPUT);
    }
  for (int thisPin = 0; thisPin < pinCount; thisPin++) {
    pinMode(ledPins[thisPin], OUTPUT);
  }
//  pinMode(1,OUTPUT);
}

void loop() {
   //digitalWrite(1, HIGH); 
    
   Serial.print(digitalRead(swch));
    Serial.print(digitalRead(lrn));

  for(int i=0;i<butCount;++i)
  {
  Serial.print(digitalRead(but[i]));
  }
  Serial.println();
  if( digitalRead(swch)==HIGH)
  {
   state=0;
    analogWrite(A0, state);
  for(int j=0;j<count;++j)
  {
   for(int i=0;i<pinCount;++i)
  {
    //Serial.println(memory[j][i]);
    digitalWrite(ledPins[i], memory[j][i]); 
  }
  delay(300);
  }
   for(int i=0;i<pinCount;++i)
  {
    
    digitalWrite(ledPins[i], LOW); 
  }
 
  analogWrite(A1,255);
  delay(100);
  analogWrite(A1,0);
  
  
  //analogWrite(A1, 0);
  
  }
  else
  {
  while(digitalRead(lrn)==0);
  while(digitalRead(lrn)==1);
  state=255;
   analogWrite(A0, state);
   for(int i=0;i<butCount;++i)
  {
   memory[count][i]=digitalRead(but[i]); 
  }
    for(int i=0;i<pinCount;++i)
  {
    digitalWrite(ledPins[i], digitalRead(but[i])); 
  }
  delay(500);
   for(int i=0;i<pinCount;++i)
  {
    digitalWrite(ledPins[i], LOW); 
  }
  
  Serial.print("hai::");
   Serial.println(count);
  for(int j=0;j<=count;++j)
  {
   for(int i=0;i<butCount;++i)
  {
   Serial.print(memory[j][i]); 
  }
   Serial.println();
  }
count=count+1;
 Serial.println("------");
if(count==limit)
{
  count=0;
}
}

  
}
  




void fun(int m)
{
   for (int thisPin = 0; thisPin < pinCount; thisPin++) {
    // turn the pin on:
    digitalWrite(ledPins[thisPin], HIGH);
    digitalWrite(ledPins[(thisPin+m)%mod], HIGH);
    delay(timer);
    // turn the pin off:
    digitalWrite(ledPins[thisPin], LOW);
    digitalWrite(ledPins[(thisPin+m)%mod], LOW);

  }

  // loop from the highest pin to the lowest:
  for (int thisPin = pinCount - 1; thisPin >= 0; thisPin--) {
    // turn the pin on:
    digitalWrite(ledPins[thisPin], HIGH);
    digitalWrite(ledPins[(thisPin+m)%mod], HIGH);
    delay(timer);
    // turn the pin off:
    digitalWrite(ledPins[thisPin], LOW);
    digitalWrite(ledPins[(thisPin+m)%mod], LOW);
  }
}
  // loop from the lowest pin to the highest:
//  for (int thisPin = 0; thisPin < pinCount; thisPin++) {
//    // turn the pin on:
//    digitalWrite(ledPins[thisPin], HIGH);
//    delay(timer);
//    // turn the pin off:
//    digitalWrite(ledPins[thisPin], LOW);
//
//  }
//
//  // loop from the highest pin to the lowest:
//  for (int thisPin = pinCount - 1; thisPin >= 0; thisPin--) {
//    // turn the pin on:
//    digitalWrite(ledPins[thisPin], HIGH);
//    delay(timer);
//    // turn the pin off:
//    digitalWrite(ledPins[thisPin], LOW);
//  }
//fun(0);
//fun(1);
//fun(2);
 //fun(3);
