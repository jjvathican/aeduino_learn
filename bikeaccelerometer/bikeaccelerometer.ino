int v=0;
int sum=0;
int dif=0;
int las=0;
void setup()
{
  pinMode(7, OUTPUT);
  pinMode(6,OUTPUT);
   pinMode(2,OUTPUT);
  Serial.begin(9600);
  digitalWrite(6, LOW);
}

void loop()
{
  digitalWrite(7, HIGH);
  
  do
  {
  v=map(analogRead(A0),0,1023,0,100);
  }while(v==0);
  
  
    dif=v-las;
    if(dif>8)
    {
      Serial.println("accelerating");
      Serial.println("stoping engine");
      digitalWrite(6, HIGH);
      digitalWrite(2, HIGH);
      delay(5000);
    }
    else
    {
      digitalWrite(6, LOW);
      digitalWrite(2, LOW);
      //Serial.println("engine on");
    }
    las=v; 
  
  Serial.println(v);
  //delay(20);
  
