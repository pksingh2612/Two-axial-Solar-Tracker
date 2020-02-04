#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define dirPin_A 2
#define stepPin_A 3
#define dirPin_B 4
#define stepPin_B 5

int topleft;
int topright;
int downleft;
int downright;
int waittime = 1;

RF24 radio(9, 10); // CE, CSN
const byte address[6] = "00001";

void setup() 
{
  Serial.begin(9600);
  DDRB = 0b00011111;
  pinMode(stepPin_A, OUTPUT);
  pinMode(dirPin_A, OUTPUT);
  pinMode(stepPin_B, OUTPUT);
  pinMode(dirPin_B, OUTPUT);
  
  TCCR1A = 0;
  TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM11);
  TCCR1B = 0;
  TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11);
  ICR1 = 20000;
  OCR1A = 3000;
  OCR1B = 3600;

  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}

void loop() 
{
topleft = analogRead(A0);
topright = analogRead(A1);
downleft = analogRead(A2);
downright = analogRead(A3);


int avgTop=(topleft+topright)/2;
int avgDown=(downleft+downright)/2;
int avgLeft=(topleft+downleft)/2;
int avgRight=(topright+downright)/2;

int dvert=avgTop-avgDown;
int dhor=avgLeft-avgRight;

Serial.print("TL");Serial.print(",");Serial.print("TR");Serial.print(",");Serial.print("DL");Serial.print(",");Serial.print("DR");Serial.print(",");Serial.print("AT");Serial.print(",");Serial.print("AD");Serial.print(",");Serial.print("AL");Serial.print(",");Serial.println("AR");
Serial.print(topleft);Serial.print(",");Serial.print(topright);Serial.print(",");Serial.print(downleft);Serial.print(",");Serial.print(downright);Serial.print(",");Serial.print(avgTop);Serial.print(",");Serial.print(avgDown);Serial.print(",");Serial.print(avgLeft);Serial.print(",");Serial.println(avgRight);

int text[] = {topleft,topright,downleft,downright,avgTop,avgDown,avgLeft,avgRight};
radio.write(&text, sizeof(text));

  if (topleft > topright) 
  {
    OCR1A = OCR1A + 1;
    digitalWrite(dirPin_A, LOW);
    digitalWrite(stepPin_A, HIGH); 
    delayMicroseconds(2000);
    digitalWrite(stepPin_A, LOW);
    delayMicroseconds(2000);
  }
  if (downleft > downright) 
  {
    OCR1A = OCR1A + 1;
    digitalWrite(dirPin_A, LOW);
    digitalWrite(stepPin_A, HIGH); 
    delayMicroseconds(2000);
    digitalWrite(stepPin_A, LOW);
    delayMicroseconds(2000);
  }
  if (topleft < topright) 
  {
    OCR1A = OCR1A - 1;
    digitalWrite(dirPin_A, HIGH);
    digitalWrite(stepPin_A, HIGH); 
    delayMicroseconds(2000);
    digitalWrite(stepPin_A, LOW);
    delayMicroseconds(2000);
  }
  if (downleft < downright) 
  {
    OCR1A = OCR1A - 1;
    digitalWrite(dirPin_A, HIGH);
    digitalWrite(stepPin_A, HIGH); 
    delayMicroseconds(2000);
    digitalWrite(stepPin_A, LOW);
    delayMicroseconds(2000);
  }
  if (OCR1A > 2000) {
    OCR1A = 2000;
  }
  if (OCR1A < 2000) {
    OCR1A = 2000;
  }
  if (topleft > downleft) 
  {
    OCR1B = OCR1B - 1;
    digitalWrite(dirPin_B, LOW);
    digitalWrite(stepPin_B, HIGH); 
    delayMicroseconds(2000);
    digitalWrite(stepPin_B, LOW);
    delayMicroseconds(2000);
  }
  if (topright > downright) 
  {
    OCR1B = OCR1B - 1;
    digitalWrite(dirPin_B, LOW);
    digitalWrite(stepPin_B, HIGH); 
    delayMicroseconds(2000);
    digitalWrite(stepPin_B, LOW);
    delayMicroseconds(2000);
  }
  if (topleft < downleft) 
  {
    OCR1B = OCR1B + 1;
    digitalWrite(dirPin_B, HIGH);
    digitalWrite(stepPin_B, HIGH); 
    delayMicroseconds(2000);
    digitalWrite(stepPin_B, LOW);
    delayMicroseconds(2000);
  }
  if (topright < downright) 
  {
    OCR1B = OCR1B + 1;
    digitalWrite(dirPin_B, HIGH);
    digitalWrite(stepPin_B, HIGH); 
    delayMicroseconds(2000);
    digitalWrite(stepPin_B, LOW);
    delayMicroseconds(2000);
  }
  if (OCR1B > 4200) {
    OCR1B = 4200;
  }
  if (OCR1B < 3000) {
    OCR1B = 3000;
  }
  delay(1000);
}
