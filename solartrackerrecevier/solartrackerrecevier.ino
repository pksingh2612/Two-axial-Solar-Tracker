#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(9, 10); // CE, CSN
const byte address[6] = "00001";
int text[32]; 
//int text[32];
void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}
void loop() {
  if (radio.available()) {
radio.read(&text,sizeof(text));
//Serial.println(c);
    //radio.read(&text, sizeof(text));
    //DR,DL,TR,TL,AP,AR
    //Serial.print("DR");Serial.print(",");Serial.print("DL");Serial.print(",");Serial.print("TR");Serial.print(",");Serial.println("TL");//Serial.print(",");Serial.print("AT");Serial.print(",");Serial.print("AD");Serial.print(",");Serial.print("AL");Serial.print(",");Serial.println("AR");
    Serial.print(text[0]);Serial.print(",");Serial.print(text[1]);Serial.print(",");Serial.print(text[2]);Serial.print(",");Serial.print(text[3]);Serial.print(",");Serial.println(text[4]);
    }
  delay(1000);
}
