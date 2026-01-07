int speed = 9000;
bool prevState = 0;
bool motor = false;
bool buttonState = false;

void setup() {
  Serial.begin(9600);

  pinMode(A0, OUTPUT);
  digitalWrite(A0, LOW);

  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);

  digitalWrite(6,LOW);
  
  prevState = analogRead(A6) > 800;
  
}

int i = 0;
void loop() {
  buttonState = analogRead(A6) > 800;

  if (prevState != buttonState && buttonState)
  {
    motor = !motor;
  }
  prevState = buttonState;

  if((i < speed) && motor)
  {
    digitalWrite(A0, LOW);
    tone(7,i);
    delayMicroseconds(500);
    i+=1;
  }
  else if(!motor && i > 0)
  {
    tone(7,i);
    delay(1);
    i -= 10;
  }
  if(i <= 0 && !motor)
  {
    digitalWrite(A0, HIGH);
    tone(7,0);
    i = 0;
  }
}
