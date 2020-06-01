/*
### Read SMS messages
 1. This example is used to test DFRobot_SIM808 GPS/GPRS/GSM Shield's reading SMS
 2. Open the SIM808_SMSread example or copy these code to your project
 3. Download and dial the function switch to Arduino
 4. it will receive SMS Messages and send it to serial

create on 2016/09/23, version: 1.0
by jason
*/

#include <DFRobot_sim808.h>
#include <SoftwareSerial.h>
#include <Metro.h> 
#define MESSAGE_LENGTH 160
char message[MESSAGE_LENGTH];
int messageIndex = 0;
char receivedChar;
boolean sendsms=0;
boolean newData = false;
boolean gpsset=false;

char MESSAGE[300];
char phone[16];
char datetime[24];
char lat[12]="           ";
char lon[12]="           ";
char wspeed[12]="            ";
int relay=6;
int s=0;
int s1=0;
String str[10];
String str1[10];
#define PIN_TX    10
#define PIN_RX    11
SoftwareSerial mySerial(PIN_TX,PIN_RX);
DFRobot_SIM808 sim808(&mySerial);//Connect RX,TX,PWR,
Metro metro0 = Metro(240000); 
Metro metro1 = Metro(200); 
Metro metro2 = Metro(400);
Metro metro3 = Metro(1000);
//DFRobot_SIM808 sim808(&Serial);

void setup() {
  mySerial.begin(9600);
  Serial.begin(9600);
 pinMode(relay, OUTPUT);
digitalWrite(relay, LOW);
  //******** Initialize sim808 module *************
  while(!sim808.init()) {
      Serial.print("Sim808 init error\r\n");
      delay(1000);
  }
    while(!sim808.attachGPS())
    {
      Serial.println("Open the GPS power failure");
      delay(1000);
    }
 
}

int fun()
{

  Serial.println("((");
  while(!sim808.getGPS()) 
  {   
  recvOneChar();
  showNewData(); 
  }
  Serial.println();
  Serial.println("))");
  
   
}

void loop() {

  
  if (metro0.check() == 1) 
  {
  if(gpsset==false)
  {
    fun();
    gpsset=true;
  }
  }

  if(metro1.check()==1 )
  {
    str[s]="";
   messageIndex = sim808.isSMSunread();
   if(messageIndex>1)
     {
   
      sim808.readSMS(messageIndex, message, MESSAGE_LENGTH, phone, datetime);
      sim808.deleteSMS(messageIndex);
      

      str[s]=str[s]+":";
      str[s]=str[s]+phone;
      str[s]=str[s]+":";
      str[s]=str[s]+message;
      
       if(String(message) == "start")
      {
       str[s]=str[s]+":";
       str[s]=str[s]+1;
       digitalWrite(relay, HIGH);
      }
       else if(String(message) == "stop")
      {
      str[s]=str[s]+":";
      str[s]=str[s]+0;
      digitalWrite(relay, LOW);
      }   
      else
      {
         sendsms=1;
      }
     }
     else
     {
      str[s]=str[s]+":::";
     } 
   // Serial.println(str[s]);
     s=s+1;
     if(s==10)
     {
      s=0;
     }
  }

  
  if(metro2.check()==1)
  {
    str1[s1]="";
    if(gpsset==true)
    {
      fun();
      str1[s1]=str1[s1]+":";
      str1[s1]=str1[s1]+sim808.GPSdata.lat;;
      str1[s1]=str1[s1]+":";
      str1[s1]=str1[s1]+sim808.GPSdata.lon;;
      str1[s1]=str1[s1]+":";
      str1[s1]=str1[s1]+sim808.GPSdata.speed_kph; 
  
      
    }
    else
    {
      str1[s1]=str1[s1]+":::";
    }
    // Serial.println(str1[s1]);
     s1=s1+1; 
     if(s1==10)
     {
      s1=0;
     }   
   }
  if(metro3.check()==1)
  {
   // Serial.println(((s1-1)+10)%10);
    
   Serial.println(str[((s-1)+10)%10]+str1[((s1-1)+10)%10]);
  }
  
 }

void recvOneChar() {
 if (Serial.available() > 0) {
 receivedChar = Serial.read();
 newData = true;
 }
}

void showNewData() {
 if (newData == true) { 
 Serial.print(receivedChar);
 newData = false;
 }
}

