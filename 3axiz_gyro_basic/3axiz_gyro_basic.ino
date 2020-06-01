#include<Wire.h>

int MPU=0x68;
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;

void setup()
{
  Serial.begin(115200);
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B); 
  Wire.write(0);    
  Wire.endTransmission(true);
}


void gxycall()
{
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
 AcX = map(AcX, -15000, 10000, 0, 1920);
 AcY = map(AcY, -15000, 10000, 0, 1080);
  AcZ = map(AcZ, -10000, 10000, 0, 1000);
}
void gxyprint()
{
 
Serial.println(AcX);
//Serial.println("|"); 
//Serial.println(AcY);
//  Serial.print(" | Z = "); Serial.println(AcZ);  
// //  Serial.print(" | Addup = "); Serial.println(addup);   
//  Serial.print("Gyroscope: ");
//  Serial.print("X = "); Serial.print(GyX);
//  Serial.print(" | Y = "); Serial.print(GyY);
//  Serial.print(" | Z = "); Serial.println(GyZ);
//  Serial.println(" "); 
}
void loop(){
  Serial.println("hello");
gxycall();
gxyprint();
delay(100);
}
