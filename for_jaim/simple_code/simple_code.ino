
/* A simple program to sequentially turn on and turn off 12 LEDs */ 

int LED[] = {1,2,3,4,5,6,7,8,9,10,11,12};
int j=2;
int c=0;



void setup() {
  Serial.begin(9600);
  for (int thisPin = 0; thisPin < 12; thisPin++)
  {
    pinMode(LED[thisPin], OUTPUT);
  }
}
void loop() {
  c=random(1,4);
  j=random(1,4);
  for(int i=0;i<12;++i)
  {
     if((i%c)%j==0)
    {
    Serial.print('*');
    }
    else
    {
      Serial.print(0);
    }
  }

 for (int thisPin = 0; thisPin < 12; thisPin++)
  {
    if((thisPin%2)==0)
    {
    digitalWrite(LED[thisPin], HIGH);
    }
    else
    {
       digitalWrite(LED[thisPin], LOW);
    }
  }
//delay(.10);  
  Serial.println();
  for(int i=0;i<12;++i)
  {
    Serial.print(0);
  }
  
 for (int thisPin = 0; thisPin < 12; thisPin++)
  {
    digitalWrite(LED[thisPin], LOW);
  }
  delay(1000);
   Serial.println();
  
   
}
