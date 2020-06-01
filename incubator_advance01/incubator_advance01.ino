
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
int minuteA=0;
int secondA=0;
int place=0;
int okstate=1;
float tempC=0;
float humP=0;
int speaker  =7;

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
    lcd.print("ERROR:              ");
    lcd.setCursor(0,1);
    lcd.print("TEM SENSOR ERROR     ");
    digitalWrite(temp_relay,1);
    okstate=0;
   
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
       tone(speaker,5000, 50);
//         lcd.setCursor(0,0);
//         lcd.print("ERROR:              ");
//         lcd.setCursor(0,1);
//         lcd.print("LOW TEMPERATURE     ");  
   //      okstate=0;       
      }
  }
  else
  {
      if(tempC>(val+1))
      {
      tone(speaker,5000, 50);
//         lcd.setCursor(0,0);
//         lcd.print("ERROR:              ");
//         lcd.setCursor(0,1);
//         lcd.print("HIGH TEMPERATURE     ");
//         okstate=0;     
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
  lcd.print("Set OFFTime(H)");
   lcd.setCursor(0,1);
   lcd.print("HOUR:");
   timecounter=timecounter%200;
  p3dig(timecounter);
  EEPROM.update(10,timecounter);  
  
}

void menu4()
{
  lcd.setCursor(0,0);
  lcd.print("Set ONTime(M:S)");
  lcd.setCursor(0,1);
  p2dig(minuteA);
  lcd.print(":");
  p2dig(secondA);
  if(place==0)
  {
  lcd.noBlink();   
  timecounter=minuteA;
  place=place+1;
  }
  else if(place==1)
  {
  lcd.setCursor(0,1); 
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
   lcd.setCursor(3,1); 
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
 if (displayMetro.check() == 1) 
  {   
     
if(menu==0)
{
 if (dellasMetro.check() == 1 ) 
  { 
  okstate=1;
 sensors.requestTemperatures(); // Send the command to get temperatures
  Serial.println("DONE");
  if(sensors.getAddress(tempDeviceAddress, 0))
  {
    Serial.print("Temperature for device: ");
    Serial.println(0,DEC);
    printTemperature(tempDeviceAddress);
  }
  else
  {
     lcd.setCursor(0,0);
    lcd.print("ERROR:              ");
    lcd.setCursor(0,1);
    lcd.print("TEM SENSOR ERROR     ");
    digitalWrite(temp_relay,1);
    okstate=0;
  
  }
  }
  if (dhtMetro.check() == 1) 
  { 
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
  }
  else {
    Serial.print(F("Temperature: "));
    Serial.print(event.temperature);
    Serial.println(F("°C"));
  }
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity) ) {
    Serial.println(F("Error reading humidity!"));
    lcd.setCursor(0,0);
    lcd.print("ERROR:              ");
    lcd.setCursor(0,1);
    lcd.print("HUM SENSOR ERROR     ");
    digitalWrite(hum_relay,1);   
   okstate=0;
    
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
    num=60000;
  }
  cradleMetro=Metro(num);
  minuteA=EEPROM.read(11);
  secondA=EEPROM.read(12);
  unsigned long milliseconds=10000;
  if(minuteA==0)
  {
   milliseconds=secondA*1000; 
  }
  else if(secondA==0)
  {
  milliseconds=minuteA*60*1000;  
  }
  else
  {
 milliseconds=(minuteA*60*1000)+(secondA*1000); 
  }
  waitfun=millis()+milliseconds;
  }
  if(waitfun>millis())
    {
       digitalWrite(cradle,0);
    }
    else
    {
      digitalWrite(cradle,1);
    }
  if(okstate==1)
  { 
  lcd.setCursor(0,0);
  lcd.print("Temp:");//  Humi  CRA  ");
  lcd.setCursor(5,0);
  lcd.print(tempC);
  lcd.setCursor(0,1);
  lcd.print("Humi:");
  lcd.setCursor(5,1);
  p2dig(int(humP));
  lcd.print("%");
  lcd.setCursor(12,0);
  lcd.print("CRD");
  lcd.setCursor(12,1);
  if(digitalRead(cradle)==1)
  {
  lcd.print("OFF");
  }
  else
  {
   lcd.print("ON "); 
  }
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
   lcd.clear();
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

  Serial.println("DOUBLE");
  menu=menu+1;
}
void singleclick() {

  Serial.println("SINGLE");
  place=place+1;
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
   
    counter=counter+.1;
    
   
    timecounter=timecounter+1;
    
    Serial.println("CLOCKWISE");
  }
  else if (result == DIR_CCW) {
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
