int acPin = 9; 
 int virgrd = 10;
 double val = 0 ; 
 int freq = 50;
 double t = 0;
const double pi = 3.1415;
 const double fs=1000;
int analog1 = A0;  // Analog input pin that the potentiometer is attached to
 int analog2 =A2; 
 int analog3 =A4;
 int analog4 =A5;
void setup() {

pinMode(acPin, OUTPUT);
Serial.begin(9600);

}
void fun1(double pos) 
{
  
   analogWrite(analog1, 0);
   analogWrite(analog2, 0);
   analogWrite(analog3, pos);
   analogWrite(analog4, pos);
}
void fun2(double pos)
{
   analogWrite(analog3, 0);
   analogWrite(analog4, 0);
   analogWrite(analog1, pos);
   analogWrite(analog2, pos);
  
}
void loop()
{
t = millis();
 val = 127*sin(2*pi*(freq/fs)*t)+127; 
 analogWrite(acPin,val);
 analogWrite(virgrd,127);
 if(val>127)
 {
  fun1(val);
 }
 else if(val<127)
 {
  fun2(val);
 }
// Serial.println(val);

}
