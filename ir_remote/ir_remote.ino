#include <boarddefs.h>
#include <IRremote.h>
#include <IRremoteInt.h>
#include <ir_Lego_PF_BitStreamEncoder.h>
 int unsigned value=0;
int receiver_pin = 3;
IRrecv receiver(receiver_pin);
decode_results output;
void setup() 
{
 
  pinMode(2, OUTPUT);
  int r_pin = 3;
  Serial.begin(9600);
  receiver.enableIRIn();
}

// the loop function runs over and over again forever
void loop() {
  
  if (receiver.decode(&output)) 
  {
    value = output.value;
    delay(100);   
    Serial.println(value);
  }
  
//  digitalWrite(2, HIGH);   // turn the LED on (HIGH is the voltage level)
//  delay(1000);                       // wait for a second
//  digitalWrite(2, LOW);    // turn the LED off by making the voltage LOW
//  delay(1000);                       // wait for a second
}
