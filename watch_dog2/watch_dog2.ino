#include <WatchDog.h>
byte WDTcounts;
void(* resetFunc) (void) = 0;//declare reset function at address 0

void setup() {
Serial.begin(9600);
Serial.println("hello");
WDTcounts=0;
WatchDog::init(WDTint_, 500);

}

void loop() {
  while(1)
  {
Serial.println("balaa");
delay(100);
  }
  
WDTcounts=0; // if you remove it your system will RESET

}

void WDTint_() {                                         // watchdog timer interrupt service routine    
WDTcounts=WDTcounts+1;
if(WDTcounts>10)
{
    WDTcounts=0;
    resetFunc();
}
}
