
int timer = 100;           // The higher the number, the slower the timing.
int ledPins[] = { 2, 7, 4, 6, 5, 3};       // an array of pin numbers to which LEDs are attached
int pinCount = 6;           // the number of pins (i.e. the length of the array)


 void s_monit(int pin)
  {
   for(int i=0;i< pinCount;++i)
    {
     if(i==pin)
     { 
      Serial.print(1);
     }
     else
     {
     Serial.print(0);
     }
    }
      Serial.println();
  }


  void setup()
 {
  Serial.begin(9600); 
  for (int thisPin = 0; thisPin < pinCount; thisPin++)
  {
    pinMode(ledPins[thisPin], OUTPUT);
  }
}

void loop() {
 int s=0;
  for (int thisPin = 0; thisPin < pinCount; thisPin++)
  {
   
    s_monit(thisPin);
    digitalWrite(ledPins[thisPin], HIGH);
    delay(timer);
    digitalWrite(ledPins[thisPin], LOW);

  }
 
  for (int thisPin = pinCount - 1; thisPin >= 0; thisPin--) 
  {
    s_monit(thisPin);
    digitalWrite(ledPins[thisPin], HIGH);
    delay(timer);
    digitalWrite(ledPins[thisPin], LOW);
  }
}

