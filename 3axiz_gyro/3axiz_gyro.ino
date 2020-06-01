#include<Wire.h>
#include <Metro.h>
 
Metro ledMetro = Metro(5000); 
int MPU=0x68;
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
int led=13;
int relay=10;
//int resetit=7;
//int x1=0;
//int y1=0;
//int z1=0;
//
//int sped=15;
int addup=0;
//int zls=0;
//int zgr=0;
//int yls=0;
//int ygr=0;
//int xls=0;
//int xgr=0;
void setup()
{
  //digitalWrite(resetit,HIGH);
  //pinMode(resetit,OUTPUT);
  pinMode(led,OUTPUT);
  pinMode(relay,OUTPUT);
  Serial.begin(9600);
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B); 
  Wire.write(0);    
  Wire.endTransmission(true);
}
void(* resetFunc)(void)=0;
//void setupit()
//{
//  
//  // digitalWrite(resetit,LOW);
//  
//}
void engine_break(int d)
{
   digitalWrite(led,HIGH);
   digitalWrite(relay,HIGH);
   delay(d*1.3);
   digitalWrite(led,LOW);
   digitalWrite(relay,LOW);
   delay(d*.8);
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
  AcX = map(AcX, -30000, 30000, 50, 150);
  AcY = map(AcY, -30000, 30000, 50, 150);
  AcZ = map(AcZ, -30000, 30000, 50, 150);
}
void gxyprint()
{
  Serial.print("Accelerometer: ");
  Serial.print("X = "); Serial.print(AcX);
  Serial.print(" | Y = "); Serial.print(AcY);
  Serial.print(" | Z = "); Serial.println(AcZ);  
   Serial.print(" | Addup = "); Serial.println(addup);   
//  Serial.print("Gyroscope: ");
//  Serial.print("X = "); Serial.print(GyX);
//  Serial.print(" | Y = "); Serial.print(GyY);
//  Serial.print(" | Z = "); Serial.println(GyZ);
//  Serial.println(" "); 
}
void loop(){
if (ledMetro.check() == 1) 
{
   resetFunc();
}
addup=addup+1;
gxycall();
gxyprint();
if(AcZ>108)
{
  engine_break(AcZ);
}
// addup=0;
// x1=0;
// y1=0;
// z1=0;
// xls=0;
// xgr=0;
// yls=0;
// ygr=0;
// zls=0;
// zgr=0;
//  while(x1<AcX && xls<sped)
//  {
//    x1 = AcX;
//   gxycall();
//   xls=xls+1;
//  } 
//  while(x1>AcX && xgr<sped)
//  {
//    x1 = AcX;
//   gxycall();
//   xgr=xgr+1;
//  } 
//  
//  while(y1<AcY && yls<sped)
//  {
//    y1 = AcY;
//   gxycall();
//   yls=yls+1;
//  }
//  while(y1>AcY && ygr<sped)
//  {
//    y1 = AcY;
//    gxycall();
//    ygr=ygr+1;
//  } 
//  while(z1<AcZ && zls<sped)
//  {
//    z1 = AcZ;
//    gxycall();
//    zls=zls+1;
//  }
//  while(z1>AcZ && zgr<sped)
//  {
//   z1 = AcZ;
//   gxycall();
//   zgr=zgr+1; 
//  }
//  
//  if(xls==xgr||yls==ygr||zls==zgr)
//  {
//     Serial.println("do nothing");
//  }
//  else
//  {
//    addup=xls+xgr+yls+ygr+zls+zgr;
//  if(addup>sped)
//  { 
//    
//  engine_break(1000);//addup*10);
//  }
////  Serial.println(xls);
////   Serial.println(xgr);
////    Serial.println(yls);
////     Serial.println(ygr);
////      Serial.println(zls);
////       Serial.println(zgr);
//        Serial.println(addup);
//delay(300);
//  }
}
