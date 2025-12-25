#include <SPI.h>
#include <WiFi.h>

#define MOSI D10
#define SCLK D8
#define RCLK D9
#define BUFFER_1 D0
#define BUFFER_2 D6
#define BUFFER_3 D4
#define BUFFER_4 D2
#define TRG_1 D1
#define TRG_2 D5
#define TRG_3 D3
#define TRG_4 D7

long del = 1;
bool received = false;

const uint8_t zero[] = {0,0,0,0,0};

const int RIGHE = 360;
const int COLONNE = 5;
const int angleCoeff = 1; // una striscia di immagine ogni 5 gradi. per essere cambiata deve cambiare anche image
int position = 0;         // le posizioni massime sono date da angleCoeff. in questo caso positions non puo superare 72 (71 visto che è un indice) (360/5)

SPISettings settings(20000000, MSBFIRST, SPI_MODE0);

volatile uint32_t prevTime = 0;
volatile uint32_t delta = 0;

// wifi
const char* ssid     = "ssid";
const char* password = "passwd";

IPAddress server_ip(10, 201, 3, 62); 
const uint16_t server_port = 9999;
WiFiClient client;

const size_t ARRAY_SIZE_BYTES = RIGHE * COLONNE * sizeof(uint8_t); // 1800 byte

// lampeggio led attesa connessione
unsigned long prevBlink = 0;
uint16_t blinkDelay = 600;
bool blinkStat = false;

// delay wifi
unsigned long prevWifi = 0;
uint16_t wifiDelay = 5000;

unsigned long initTime = 0;

void connectToServer() {
  Serial.print("Tentativo di connessione a ");
  Serial.print(server_ip);
  Serial.print(" sulla porta ");
  Serial.println(server_port);

  // client.connect() restituisce true se la connessione è stata stabilita
  if (client.connect(server_ip, server_port)) {
    Serial.println("Connessione TCP riuscita!");
    client.println("ciao");
  } else {
    Serial.println("Connessione TCP fallita.");
  }
}

void setup() {
  SPI.begin(SCLK, -1, MOSI);
  Serial.begin(9600);
  pinMode(TRG_2, INPUT);
  pinMode(BUFFER_1, OUTPUT);
  pinMode(BUFFER_2, OUTPUT);
  pinMode(BUFFER_3, OUTPUT);
  pinMode(BUFFER_4, OUTPUT);
  pinMode(RCLK, OUTPUT);
  SPI.beginTransaction(settings);
  
  // trg1 e trg2 sono rotti 
  attachInterrupt(TRG_3, sense, RISING);

  Serial.println();
  Serial.print("Connessione a: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  // Attendi fino a quando non è connesso
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  connectToServer();
}

void sense()
{
  uint32_t current = micros();
  delta = current - prevTime;
  prevTime = current;
  position = 283;
}

void loop() 
{
  initTime = micros();
  if(!received)
  {
    unsigned long currentTime = millis();
    if(currentTime - prevBlink > blinkDelay)
    {
        prevBlink = currentTime;
        blinkStat = !blinkStat;
        writeAllLeds(blinkStat);
    }

    if (client.available()) {
      size_t bytesRead = client.readBytes(&image[0][0], ARRAY_SIZE_BYTES);
      client.print("ACK: Ricevuto");
      received = true;
      client.stop();
    }

    if (!client.connected() && (currentTime - prevWifi > wifiDelay)) {
      Serial.println("Disconnesso dal server. Arresto e Riconnessione...");
      client.stop();
      connectToServer();
      prevWifi = currentTime;
    }
    delay(10); 
  }
  else{

    if (millis()-int(prevTime/1000) > 3000) received = false;
    writeImg(position, image);
    
    digitalWrite(RCLK, HIGH);
    digitalWrite(RCLK, LOW);
    //delayMicroseconds(del);
    position++;

    double tempoFetta = delta / 360; // * angleCoeff;
    unsigned long errorDelay = micros() - initTime;
    delayMicroseconds(tempoFetta - errorDelay);
    if(position >= 360) position = 0;
  }
}

void writeAllLeds(bool state)
{
  digitalWrite(BUFFER_1, LOW);
  digitalWrite(BUFFER_2, LOW);
  digitalWrite(BUFFER_3, LOW);
  digitalWrite(BUFFER_4, LOW);
  uint8_t arr[5] = {255 * (int)state, 255 * (int)state, 255* (int)state, 255 * (int)state, 255 * (int)state};
  Serial.println(arr[0]);
  SPI.writeBytes(arr,5);

  digitalWrite(RCLK, HIGH);
  digitalWrite(RCLK, LOW);
}

void writeImg(uint16_t pos, uint8_t img[][5])
{
  digitalWrite(BUFFER_1, LOW);
  digitalWrite(BUFFER_2, HIGH);
  digitalWrite(BUFFER_3, HIGH);
  digitalWrite(BUFFER_4, HIGH);
  SPI.writeBytes(img[pos],5);
  digitalWrite(BUFFER_1, HIGH);
  digitalWrite(BUFFER_2, LOW);
  digitalWrite(BUFFER_3, HIGH);
  digitalWrite(BUFFER_4, HIGH);
  SPI.writeBytes(img[getPosFromAngle(1,pos)],5);
  digitalWrite(BUFFER_1, HIGH);
  digitalWrite(BUFFER_2, HIGH);
  digitalWrite(BUFFER_3, LOW);
  digitalWrite(BUFFER_4, HIGH);
  SPI.writeBytes(img[getPosFromAngle(2,pos)],5);
  digitalWrite(BUFFER_1, HIGH);
  digitalWrite(BUFFER_2, HIGH);
  digitalWrite(BUFFER_3, HIGH);
  digitalWrite(BUFFER_4, LOW);
  SPI.writeBytes(img[getPosFromAngle(3,pos)],5);
}

uint16_t getPosFromAngle(uint8_t angle, uint16_t pos)   // l'angolo deve essere 1,2,3 che sono rispettivamente 90, 180, 270 gradi
{
  uint16_t val = (90*angle)+pos; // int(90 / angleCoeff) * angle + pos;
  if(val >= 360) val = val - 360;
  return val;
}
