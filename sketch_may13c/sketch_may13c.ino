#include<LiquidCrystal.h>
 LiquidCrystal lcd(12, 11,5, 4, 3,2);

#define in 14
#define out 19

int count=0;

void IN()
{
    count++;
    lcd.clear();
    lcd.print("Person In Room:");
    lcd.setCursor(0,1);
    lcd.print(count);
    delay(100);
}

void OUT()
{
    count--;
    lcd.clear();
    lcd.print("Person In Room:");
    lcd.setCursor(0,1);
    lcd.print(count);
    delay(100);
}

void setup()
{
  Serial.begin(9600);
  lcd.begin(16,2);
  lcd.print("Visitor Counter");
  delay(2000);
  pinMode(in, INPUT);
  pinMode(out, INPUT);
  lcd.clear();
  lcd.print("Person In Room:");
  lcd.setCursor(0,1);
  lcd.print(count);
}

void loop()
{  
 Serial.println(digitalRead(in));
  if(digitalRead(in))
  {
  IN();
  }
 
  

  
  if(count<=0)
  {
    lcd.clear();
    lcd.clear();
    lcd.print("Nobody In Room");
    lcd.setCursor(0,1);
    lcd.print("Light Is Off");
    delay(200);
  }
 else
 {
  Serial.println(digitalRead(out));
  if(digitalRead(out))
  {
  OUT();
  }
 }

 
  
  
}

