
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(4,5,6,7,8,9);
SoftwareSerial GSerialSent(2,3);
SoftwareSerial GSerialRecieve(11,12);
char rec;
void setup() {
  delay(1000);
  Serial.begin(9600);
  Serial.println("Keyboard Test:");
  GSerialSent.begin(400);
  GSerialRecieve.begin(400);
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
  lcd.print("    Hemangi   ");
  delay(3000);
  lcd.clear();
  GSerialSent.print('&');
}

void loop() {
 if(GSerialRecieve.available() !=0)
  {
     char output = GSerialRecieve.read();
     //GSerialRecieve.read();
      Serial.print(/*GSerialRecieve.read()*/output);
      lcd.print(/*GSerialRecieve.read()*/output);
      
      //lcd.print("Sending the recieced code");
      
      //Sending the recieved code into the transmitter
      GSerialSent.print(/*GSerialRecieve.read()*/output);

      if(output='"')
      {
        lcd.print('2');
        serial.print('2');
      }
      
   }
}
//   if (Serial.available()) {
//    // wait a bit for the entire message to arrive
//    delay(100);
//    // clear the screen
//    lcd.clear();
//    // read all the available characters
//    while (Serial.available() > 0) {
//      // display each character to the LCD
//      lcd.write(Serial.read());
//      GSerialSent.print(Serial.read());
//    }
//   }
//}
   
