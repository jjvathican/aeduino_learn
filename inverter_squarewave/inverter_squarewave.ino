 int analog1 = A0;  // Analog input pin that the potentiometer is attached to
 int analog2 =A2; 
 int analog3 =A4;
 int analog4 =A5;
void setup() {
  
}
void fun1()
{
  
   analogWrite(analog1, 0);
   analogWrite(analog2, 0);
   analogWrite(analog3, 255);
   analogWrite(analog4, 255);
}
void fun2()
{
   analogWrite(analog1, 255);
   analogWrite(analog2, 255);
   analogWrite(analog3, 0);
   analogWrite(analog4, 0);
}
void loop() 
{
fun1();
delay(10);
fun2();
delay(10);

}
