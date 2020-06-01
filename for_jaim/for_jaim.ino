
int timer = 100;           // The higher the number, the slower the timing.
int ledPins[] = { 2, 7, 4, 6, 5, 13};       // an array of pin numbers to which LEDs are attached
int pinCount = 6;           // the number of pins (i.e. the length of the array)
int i =1;

 void s_monit(int pin[])
  {
    int j=0;
   for(int i=0;i< pinCount;++i)
    {
      
     if(i==pin[j])
     { 
      j=j+1;
      Serial.print('*');
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

void loop()
{
  switch(i%10)
  {
case 1:pattern1();break;
case 2:pattern2();break;
case 3:pattern3();break;
  }
 i= i+1;
}
void pattern1()
{
  int arr[1];
  for (int thisPin = 0; thisPin < pinCount; thisPin++)
  {
    arr[0]=thisPin;
    s_monit(arr);
    digitalWrite(ledPins[thisPin], HIGH);
    delay(timer);
    digitalWrite(ledPins[thisPin], LOW);

  }
 
  for (int thisPin = pinCount - 1; thisPin >= 0; thisPin--) 
  {
    arr[0]=thisPin;
    s_monit(arr);
    digitalWrite(ledPins[thisPin], HIGH);
    delay(timer);
    digitalWrite(ledPins[thisPin], LOW);
  }
}
void pattern2()
{
int arr[pinCount];
int inc=0;
  for (int thisPin = 0; thisPin < pinCount; thisPin++)
  {
    if(thisPin/2==0)
    {
      arr[inc]=thisPin;
      digitalWrite(ledPins[thisPin], HIGH);
      inc=inc+1;
    }
    else
    {
      digitalWrite(ledPins[thisPin], LOW);
    }
  }
  s_monit(arr);
}
void pattern3()
{
  
  int arr[pinCount];
  int inc=0;
  for (int thisPin = 0; thisPin < pinCount; thisPin++)
  {
    if(thisPin/2==0)
    {
      digitalWrite(ledPins[thisPin], LOW);
    }
    else
    {
      arr[inc]=thisPin;
      digitalWrite(ledPins[thisPin], HIGH);
      inc=inc+1;
    }
  }
  s_monit(arr);
}

