#include <avr/wdt.h>

void setup()
{
  delay(10000);
  Serial.begin(9600);
  Serial.println("resteing");
  wdt_disable();
  delay(2000);
  //wdt_enable(WDTO_1S);// 1 sec
 // wdt_enable(WDTO_2S);// 2 sec
  //wdt_enable(WDTO_4S);// 4 sec
  wdt_enable(WDTO_8S);// 8 sec
}

void loop()
{
  wdt_reset();
  while(1)
  {
  }
  
  
  //  your program
//  ........
}
