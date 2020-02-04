#include <EEPROM.h>
void setup() {
   Serial.begin(9600);
   EEPROM.write(0,0);
   EEPROM.write(1,0);
}

void loop() {
}
