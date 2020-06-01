
#include <OneButton.h>
OneButton button(A0, true);
void setup()
{

  Serial.begin(9600);
  button.attachDoubleClick(doubleclick);
  button.attachClick(singleclick);
  button.attachPress(longclick);
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
