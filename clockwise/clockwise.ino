
#include <OneButton.h>
#include <Rotary.h>
Rotary r = Rotary(2, 3);
OneButton button(A0, true);
void setup()
{

  Serial.begin(115200);
  //rotory interrupt
  PCICR |= (1 << PCIE2);
  PCMSK2 |= (1 << PCINT18) | (1 << PCINT19);
  sei();
  // click on the encoder
  button.attachDoubleClick(doubleclick);
  button.attachClick(singleclick);
  button.attachPress(longclick);
  Serial.println("Testing Rotary with ili9341 tft");
}
void loop()
{
  // keep watching the push button:
  button.tick();
  // You can implement other code in here or just wait a while
}
void doubleclick() {

  Serial.println("DOUBLE");
}
void singleclick() {

  Serial.println("SINGLE");
}
void longclick() {

  Serial.println("LONG");
}
ISR(PCINT2_vect)
{
  unsigned char result = r.process();
  if (result == DIR_NONE) {
    // do nothing
  }
  else if (result == DIR_CW) {

    Serial.println("CLOCKWISE");
  }
  else if (result == DIR_CCW) {

    Serial.println("COUNTERCLOCK");
  }
}
