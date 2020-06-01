#include<Wire.h>
const int MPU=0x68; 
int relay=7;
int count=0;
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;

void setup(){
  pinMode(relay,OUTPUT);
  digitalWrite(relay,HIGH);
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B); 
  Wire.write(0);    
  Wire.endTransmission(true);
  Serial.begin(9600);
}
void loop(){
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);  
  Wire.endTransmission(false);
  Wire.requestFrom(MPU,12,true);  
  AcX=Wire.read()<<8|Wire.read();    
  AcY=Wire.read()<<8|Wire.read();  
  AcZ=Wire.read()<<8|Wire.read();  
  GyX=Wire.read()<<8|Wire.read();  
  GyY=Wire.read()<<8|Wire.read();  
  GyZ=Wire.read()<<8|Wire.read();  
//  
//  Serial.print("Accelerometer: ");
//  Serial.print("X = "); Serial.print(AcX);
//  Serial.print(" | Y = "); Serial.print(AcY);
//  Serial.print(" | Z = "); Serial.println(AcZ); 
//  
  Serial.print("Gyroscope: ");
 // Serial.print("X = "); Serial.print(GyX);
  Serial.print(" | Y = "); Serial.print(GyY);
//  Serial.print(" | Z = "); Serial.println(GyZ);
  if(GyY<1000&&GyY>-1000)
  {
    count=count+1;
  }
  else
  {
    count=0;
  }
  if(count>5)
  {
    digitalWrite(relay,LOW);
    delay(100);
    count=0;
  }
  else
  {
       digitalWrite(relay,HIGH);
  }
  Serial.println(" ");
  delay(333);
}
