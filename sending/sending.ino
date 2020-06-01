#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
#include <Keypad.h>
LiquidCrystal lcd(4,5,6,7,8,9);
const byte ROWS = 4;
const byte COLS = 4;

char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {12,13,A0,A1}; 
byte colPins[COLS] = {A2,A3,A4,A5}; 

SoftwareSerial GSerialSent(2,3);
SoftwareSerial GSerialRecieve(11,12);

Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 
char keycount=0;
char code[5];

// DAve added a test comment
char rec;
void setup() {
  delay(1000);
  Serial.begin(9600);
  Serial.println("Keyboard Test:");
  GSerialSent.begin(400);
  GSerialRecieve.begin(4800);
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.setCursor(0, 0);
  lcd.print(" LiFi - Wireless");
  lcd.setCursor(0, 1);
  lcd.print(" Communication  ");
  delay(3000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" using Light    ");
  lcd.setCursor(0, 1);
  lcd.print(" Transimitter ");
  delay(3000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("  By Team Jithin &  ");
  lcd.setCursor(0,1); 
  lcd.print("    Hemangi and jj hehe poda patti sheyyy");
  delay(3000);
  lcd.clear();
  GSerialSent.print('&');
}

void loop() {
  char customKey = customKeypad.getKey();
  //char input=Serial.read();
  
  if (Serial.available()) {
    // wait a bit for the entire message to arrive
    delay(1000);
    // clear the screen
    lcd.clear();
    // read all the available characters
    while (Serial.available() > 0) {
      // display each character to the LCD
      char input = Serial.read();
      //Using a same variable to access the data file
      lcd.write(input);
      GSerialSent.print(input);
    }
  }

if((customKey && (customKey !='=')))
  {      
    if (customKey == '#') 
    {
      //Serial.print();
      GSerialSent.print('^');
      lcd.setCursor(0, 1);
      lcd.print("                ");
      lcd.setCursor(0, 1);
    } 
    else if (customKey == '*') 
    {
      GSerialSent.print('&');
      lcd.clear();
    } 
    else 
    {
      // otherwise, just print all normal characters
      Serial.print(customKey);
      GSerialSent.print(customKey);
      lcd.print(customKey);
    }
  }
  if(GSerialRecieve.available() !=0)  {
    // lcd.print("Recieving the same code");
     rec=GSerialRecieve.read();
     Serial.print(rec);
     
     lcd.print(rec);
   }
  }
  

    
