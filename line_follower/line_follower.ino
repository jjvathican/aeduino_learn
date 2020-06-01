void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
pinMode(4, INPUT);
pinMode(5, INPUT);
}

void loop() {
   Serial.print("a..");
  Serial.println(digitalRead(4));
  Serial.print("b..");
   Serial.println(digitalRead(5));
delay(1000);
}
