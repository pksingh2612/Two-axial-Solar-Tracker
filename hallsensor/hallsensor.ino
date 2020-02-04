#define hall_sensor1 0
//#define hall_sensor2 1
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
pinMode(hall_sensor1,INPUT);
//pinMode(hall_sensor2,INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
int x=digitalRead(hall_sensor1);
//int y=digitalRead(hall_sensor2);
Serial.println(x);//Serial.print(",");//Serial.println(y); //LIGHT ON --value 0 and vice-versa.
delay(5000);
}
