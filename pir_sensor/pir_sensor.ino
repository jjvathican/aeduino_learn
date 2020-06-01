int pirSensor = 22;
void setup() {
  Serial.begin(9600);
  pinMode(pirSensor, INPUT);  
}
void loop() {
  int sensorValue = digitalRead(pirSensor);
  Serial.println(sensorValue);
  if (sensorValue == 1) {
   // Serial.println("found");
  }
  //delay(100);
}
