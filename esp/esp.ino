#include <SPI.h>
#include "images.h"

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

const uint8_t zero[] = {0,0,0,0,0};

const int RIGHE = 360;
const int COLONNE = 5;
const int angleCoeff = 1; // una striscia di immagine ogni 5 gradi. per essere cambiata deve cambiare anche image
int position = 0;         // le posizioni massime sono date da angleCoeff. in questo caso positions non puo superare 72 (71 visto che è un indice) (360/5)

SPISettings settings(20000000, MSBFIRST, SPI_MODE0);

volatile uint32_t prevTime = 0;
volatile uint32_t delta = 0;

unsigned long initTime = 0;

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
  writeImg(position, image);
  
  digitalWrite(RCLK, HIGH);
  digitalWrite(RCLK, LOW);
  position++;
  if (position > 360) position = 0;

  double tempoFetta = delta / 360; // * angleCoeff;
  unsigned long errorDelay = micros() - initTime;
  delayMicroseconds(tempoFetta - errorDelay);
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
