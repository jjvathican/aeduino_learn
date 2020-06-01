
#include <OneButton.h>
#include <Rotary.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>
#include <Metro.h>

#define ONE_WIRE_BUS 5
#define TEMPERATURE_PRECISION 5
#define DHTTYPE    DHT11
#define DHTPIN 4 

Metro dellasMetro = Metro(1800);
Metro dhtMetro = Metro(1700);
Metro cradleMetro=Metro(1000);
Metro displayMetro=Metro(400);
Rotary r = Rotary(2, 3);
OneButton button(A0, true);
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress tempDeviceAddress; 
DHT_Unified dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27,16,2);

uint32_t delayMS;
int numberOfDevices;
int temp_relay=10;
int cradle=11;
int hum_relay=12;
int menu=0;
float counter=0;
int timecounter=0;
unsigned long waitfun=0;
unsigned long waitfun1=0;
int minuteA=0;
int secondA=0;
int place=0;
int tokstate=1;
int hokstate=1;
float tempC=0;
float humP=0;
int speaker  =7;
int idlecounter=0;
int menuchanger=0;
int buzzer=0;

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

  sensors.begin();
  numberOfDevices = sensors.getDeviceCount();
   Serial.print("Parasite power is: "); 
  if (sensors.isParasitePowerMode()) Serial.println("ON");
  else Serial.println("OFF");
  for(int i=0;i<numberOfDevices; i++)
  {
    // Search the wire for address
    if(sensors.getAddress(tempDeviceAddress, i))
  {
    Serial.print("Found device ");
    Serial.print(i, DEC);
    Serial.print(" with address: ");
    printAddress(tempDeviceAddress);
    Serial.println();
    
    Serial.print("Setting resolution to ");
    Serial.println(TEMPERATURE_PRECISION, DEC);
    
    // set the resolution to TEMPERATURE_PRECISION bit (Each Dallas/Maxim device is capable of several different resolutions)
    sensors.setResolution(tempDeviceAddress, TEMPERATURE_PRECISION);
    
    Serial.print("Resolution actually set to: ");
    Serial.print(sensors.getResolution(tempDeviceAddress), DEC); 
    Serial.println();
  }else{
    Serial.print("Found ghost device at ");
    Serial.print(i, DEC);
    Serial.print(" but could not detect address. Check power and cabling");
  }
  }


  dht.begin();
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  Serial.println(F("------------------------------------"));
  Serial.println(F("Temperature Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("°C"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("°C"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("°C"));
  Serial.println(F("------------------------------------"));
  // Print humidity sensor details.
  dht.humidity().getSensor(&sensor);
  Serial.println(F("Humidity Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("%"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("%"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("%"));
  Serial.println(F("------------------------------------"));
  // Set delay between sensor readings based on sensor details.
  delayMS = sensor.min_delay / 1000;
  
  digitalWrite(temp_relay,1);
  pinMode(temp_relay,OUTPUT);
  digitalWrite(hum_relay,1);
  pinMode(hum_relay,OUTPUT);
  digitalWrite(cradle,1);
  pinMode(cradle,OUTPUT);
  //digitalWrite(cradle,1);
  pinMode(speaker,OUTPUT);

  
  
  
  lcd.init();                      // initialize the lcd 
  lcd.init();
  lcd.backlight();
  lcd.noCursor();
  lcd.setCursor(0,0);
  lcd.print("     JJ TEAM    ");
  lcd.setCursor(0,1);
  lcd.print("HATCHERY  SYSTEM");
 
  delay(5000);
   lcd.clear();
 
  
}
void p2dig(int v)
// print 2 digits with leading zero
{
  if (v < 10) lcd.print("0");
  lcd.print(v);
}
void p3dig(int v)
// print 2 digits with leading zero
{
  if(v<100)
  {
    lcd.print("0");
  }
  if (v < 10) lcd.print("0");
  lcd.print(v);
}
long EEPROMReadlong(long address) {
  long four = EEPROM.read(address);
  long three = EEPROM.read(address + 1);
  long two = EEPROM.read(address + 2);
  long one = EEPROM.read(address + 3);
  
  return ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);
}

void EEPROMWritelong(int address, long value) {
  byte four = (value & 0xFF);
  byte three = ((value >> 8) & 0xFF);
  byte two = ((value >> 16) & 0xFF);
  byte one = ((value >> 24) & 0xFF);
  
  EEPROM.write(address, four);
  EEPROM.write(address + 1, three);
  EEPROM.write(address + 2, two);
  EEPROM.write(address + 3, one);
}

void printTemperature(DeviceAddress deviceAddress)
{
  
  tempC = sensors.getTempC(deviceAddress);
  if(tempC == DEVICE_DISCONNECTED_C) 
  {
    Serial.println("Error: Could not read temperature data");
    lcd.setCursor(0,0);
//    lcd.print("ERROR:              ");
//    lcd.setCursor(0,1);
//    lcd.print("TEM SENSOR ERROR     ");
    digitalWrite(temp_relay,1);
    tokstate=0;
   
    return;
  }
  else
  {
 
  Serial.print("Temp C: ");
  Serial.print(tempC);
  Serial.print(" Temp F: ");
  Serial.println(DallasTemperature::toFahrenheit(tempC)); // Converts tempC to Fahrenheit
  float val=float(EEPROMReadlong(0))/100;
  if(tempC<val)
  {
      digitalWrite(temp_relay,0);
      if(tempC<(val-2))
      {
        buzzer=1;
       tone(speaker,5000, 50);

      }
  }
  else
  {
      if(tempC>(val+1))
      {
        buzzer=1;
      tone(speaker,5000, 50);
      }
     digitalWrite(temp_relay,1);
  }
  }
}
void menu1()
{
  lcd.setCursor(0,0);
  lcd.print("Set Temperature");
   lcd.setCursor(0,1);
  lcd.print(counter);
  EEPROMWritelong(0,(counter*100));
  
}
void menu2()
{
  lcd.setCursor(0,0);
  lcd.print("Set Humidity");
   lcd.setCursor(0,1);
  lcd.print(counter);
  EEPROMWritelong(5,(counter*100));
  
}
void menu3()
{
  lcd.setCursor(0,0);
  lcd.print("Set OFFTime");
   lcd.setCursor(0,1);
   lcd.print("HOUR:");
   timecounter=timecounter%200;
  p3dig(timecounter);
  EEPROM.update(10,timecounter);  
  
}

void menu4()
{
  lcd.setCursor(0,0);
  lcd.print("Set ONTime");
  lcd.setCursor(0,1);
  lcd.print("MIN:");
  p2dig(minuteA);
  lcd.print(" SEC:");
  p2dig(secondA);
  if(place==0)
  {
  lcd.noBlink();   
  timecounter=minuteA;
  place=place+1;
  }
  else if(place==1)
  {
  lcd.setCursor(4,1); 
  lcd.blink(); 
  minuteA=timecounter%60;
  }
  else if(place==2)
  {
   lcd.noBlink(); 
   timecounter=secondA;
   place=place+1;
  }
  else if(place==3)
  {
   lcd.setCursor(11,1); 
   lcd.blink(); 
  secondA=timecounter%60; 
  }
  else if(place>3)
  {
    lcd.noBlink();
     place=0;
  }
  EEPROM.update(11,minuteA);
  EEPROM.update(12,secondA);
  
  
}
void loop()
{
  //delay(delayMS);
  button.tick();
  
 if(idlecounter>30)
 {
  lcd.noBlink();  
   lcd.clear();
  menu=0;
  idlecounter=0;
 
 }
 if (displayMetro.check() == 1) 
  {  
   if(menu>0)
   {  
 idlecounter=idlecounter+1; 

   }  
   else
   {
    idlecounter=0; 
   }
if(menu==0)
{
   menuchanger=menuchanger+1;
  
    
 if (dellasMetro.check() == 1 ) 
  { 
    
 buzzer=0;
 tokstate=1;
    
  sensors.requestTemperatures();// Send the command to get temperatures
  if(sensors.getAddress(tempDeviceAddress, 0))
  {
    Serial.print("Temperature for device: ");
    Serial.println(0,DEC);
    printTemperature(tempDeviceAddress);
  }
  else
  {
    digitalWrite(temp_relay,1);
    tokstate=0;  
  }
  }
  if (dhtMetro.check() == 1) 
  { 
     hokstate=1;
   sensors_event_t event;
  dht.humidity().getEvent(&event); 
  if (isnan(event.relative_humidity) )
  {
    Serial.println(F("Error reading humidity!"));
    digitalWrite(hum_relay,1);   
    hokstate=0;
    
  }
  else {
   
    humP=event.relative_humidity;
    Serial.print(F("Humidity: "));
    Serial.print(event.relative_humidity);
    Serial.println(F("%"));
     float val=float(EEPROMReadlong(5))/100;
  if( humP<val)
  {
      digitalWrite(hum_relay,0);
      
  }
  else
  {
     digitalWrite(hum_relay,1);
  }

    
  }
  }
  

  if (cradleMetro.check() == 1) 
  { 
  unsigned long num=int(EEPROM.read(10))*60*60*1000;
  if(num==0)
  {
    num=30000;
  }
  cradleMetro=Metro(num);
  minuteA=EEPROM.read(11);
  secondA=EEPROM.read(12);
  unsigned long milliseconds=0;
  if(minuteA==0)
  {
   milliseconds=secondA*1000UL; 
    Serial.print("millisecond:");
  Serial.println(milliseconds); 
  }
  else if(secondA==0)
  {
  milliseconds=minuteA*1000*60UL; 
  Serial.print("millisecond1:");
  Serial.println(milliseconds); 
  }
  else
  {
 milliseconds=(minuteA*60*1000)+(secondA*1000); 
  Serial.print("millisecond1:");
  Serial.println(milliseconds); 
  }
  waitfun=milliseconds;
  waitfun1=millis();
  }
  if((waitfun+waitfun1)>millis())
    {
       digitalWrite(cradle,0);
    }
    else
    {
      digitalWrite(cradle,1);
    }
  if(tokstate==1 && hokstate==1 )
  { 
  lcd.setCursor(0,0);
  lcd.print("Temp:");//  Humi  CRA  ");
  lcd.setCursor(5,0);
  lcd.print(tempC,1);
  lcd.print("C");
  lcd.setCursor(0,1);
  lcd.print("Humi:");
  lcd.setCursor(5,1);
  p2dig(int(humP));
  lcd.print("%");
  }
// if(int(menuchanger/20)%2==0)
  if(tokstate==0)
  {  
  lcd.setCursor(12,0);
  lcd.print("ERR");
  lcd.setCursor(12,1);
  lcd.print("S_T");
  }
  else if(hokstate==0)
  {
   lcd.setCursor(12,0);
  lcd.print("ERR");
  lcd.setCursor(12,1);
  lcd.print("S_H"); 
  }
 else if(buzzer==1)
 {
  lcd.setCursor(12,0);
  lcd.print("BUZ");
  lcd.setCursor(12,1);
  lcd.print("ON ");
  }
  else if(digitalRead(cradle)==0)
  {
  lcd.setCursor(12,0);
  lcd.print("CRD");
  lcd.setCursor(12,1);
  lcd.print("ON ");
  }
  else
  {
  lcd.setCursor(12,0);
  lcd.print("ALL");  
  lcd.setCursor(12,1);
  lcd.print("OFF "); 
  }
  
 
  
 
  
  
}
else if(menu==1)
  {
   OFFfunction();
    
    counter=float(EEPROMReadlong(0))/100;
    menu=menu+1; 
    lcd.clear();
  }
else if(menu==2)
  {
     menu1();  
    //menu=0;
  }
else if(menu==3)
  {
    lcd.clear();
    counter=float(EEPROMReadlong(5))/100;
    menu=menu+1; 
  }
 else if(menu==4)
 {
   menu2();
 }
 else if(menu==5)
  {
    lcd.clear();
    timecounter=EEPROM.read(10);
    menu=menu+1; 
  }
 else if(menu==6)
 {
   menu3();
 }
 else if(menu==7)
  {
    lcd.clear();
    minuteA=EEPROM.read(11);
    secondA=EEPROM.read(12);
    timecounter=0;
    menu=menu+1; 
    place=0;
  }
 else if(menu==8)
 {
   menu4();
 }
 else if(menu==9)
 {
 
   menu=menu+1;
   cradleMetro=Metro(1000);
 }
 else
 {
  lcd.noBlink();
  menu=0;
 }
  }
}
void OFFfunction()
{
  digitalWrite(hum_relay,1);
     digitalWrite(temp_relay,1);
      digitalWrite(cradle,1);
}
void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
   
  }
}
void doubleclick() {
 idlecounter=0;
  Serial.println("DOUBLE");
  if(menu>0)
  {
  menu=menu+1;
  }
}
void singleclick() {
 idlecounter=0;
  Serial.println("SINGLE");
  place=place+1;
}
void longclick() {
   idlecounter=0;
  Serial.println("LONG");
  if(menu==0)
  {
  menu=menu+1;
  }
  else
  {
    lcd.noBlink();  
     lcd.clear();
    menu=0;
  }
}
ISR(PCINT2_vect)
{
  unsigned char result = r.process();
  if (result == DIR_NONE) {
    // do nothing
  }
  else if (result == DIR_CW) {
   
    counter=counter+.1;
     idlecounter=0;
   
    timecounter=timecounter+1;
    
    Serial.println("CLOCKWISE");
  }
  else if (result == DIR_CCW) {
     idlecounter=0;
    if(counter>0)
    {
    counter=counter-.1;
    }
     if(timecounter>0)
    {
    timecounter=timecounter-1;
    }
    Serial.println("COUNTERCLOCK");
  }
}
