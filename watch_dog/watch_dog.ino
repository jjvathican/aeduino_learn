#include <avr/wdt.h>

void setup(){
   //watchdog timer with 2 Seconds time out
   Serial.begin(9600);
   Serial.println("starting again");
   delay(10000);
   wdt_enable(WDTO_8S);
}

void loop(){
   //do stuff here
    while(1)
    {
      Serial.println("hello");
    }
   wdt_reset();
}
