int choice=2;
int up=3;
int down=6;
int mode=5;
int rev=7;
void setup() {
  Serial.begin(9600);
  pinMode(choice,INPUT);
  pinMode(up,INPUT);
  pinMode(down,INPUT);
  pinMode(mode,INPUT);
  pinMode(rev,INPUT);
}

void loop() {
   Serial.println(digitalRead(mode));
    Serial.println(digitalRead(up));
     Serial.println(digitalRead(down));
      Serial.println(digitalRead(choice));
        Serial.println(digitalRead(rev));
      delay(100);
  // put your main code here, to run repeatedly:

}
