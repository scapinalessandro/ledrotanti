void setup() {
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);

  digitalWrite(3,HIGH);   // DIR PIN
  digitalWrite(4,LOW);    // ENABLE PIN

  
  for(int i = 0; i < 5000; i++)
  {
    tone(2,i);
    delay(2);
  }
  tone(2,5000);

  }
void loop() {}
