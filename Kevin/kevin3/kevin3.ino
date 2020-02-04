//Stepper Board Test Code
//Kevin Darrah  2017

//#define hall_sensor1 0
//#define hall_sensor2 1
//#include <SPI.h>  
//#include <nRF24L01.h>
//#include <RF24.h>
//#include <Wire.h>

#define enable 6

// for motor b anti-clock --up and vice versa
const int stepPin = 3;//only works on this pin right now
const int dirPin = 2;

const int actPin = 4;//not used

const float motorAngle = 1.8;
const float stepSize = 1;//full=1, half=0.5, quarter=0.25, etc...

int topleft;
int topright;
int downleft;
int downright;
int waittime = 1;

//RF24 radio(9, 10); // CE, CSN
//const byte address[6] = "00001";

void stepperRotate(float rotation, float rpm);

void setup() 
{
  // put your setup code here, to run once:
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(enable,OUTPUT);
  //pinMode(hall_sensor1,INPUT);
  //pinMode(hall_sensor2,INPUT);
  Serial.begin(9600);

  
 //int y=digitalRead(hall_sensor2);

 //if (x==0 && y==1)
 //{
 
 //for (int i=0;i<1;i++)
 //{
 //digitalWrite(enable,LOW);
 //stepperRotate(-5, 10);//rotations, RPM  for 0.25 max rpm 70 and min 5    // motor anti-clock then system clock 
 //digitalWrite(enable,HIGH); 

 //digitalWrite(enable,LOW);
 //stepperRotate(8, 5);//rotations, RPM  for 0.25 max rpm 70 and min 5    // motor anti-clock then system clock 
 //digitalWrite(enable,HIGH); 
 //}
 
 //}
 //radio.begin();
 // radio.openWritingPipe(address);
 // radio.setPALevel(RF24_PA_MIN);
 // radio.stopListening();
}

void loop() 
{
  int diff=0;
topleft = analogRead(A0);
topright = analogRead(A1);
downleft = analogRead(A2);
downright = analogRead(A3);

//int avgTop=(topleft+topright)/2;
//int avgDown=(downleft+downright)/2;
//int avgLeft=(topleft+downleft)/2;
//int avgRight=(topright+downright)/2;

//int dvert=avgTop-avgDown;
//int dhor=avgLeft-avgRight;

Serial.print("TL");Serial.print(",");Serial.print("TR");Serial.print(",");Serial.print("DL");Serial.print(",");Serial.println("DR");//Serial.print(",");Serial.print("AT");Serial.print(",");Serial.print("AD");Serial.print(",");Serial.print("AL");Serial.print(",");Serial.println("AR");
Serial.print(topleft);Serial.print(",");Serial.print(topright);Serial.print(",");Serial.print(downleft);Serial.print(",");Serial.println(downright);//Serial.print(",");Serial.print(avgTop);Serial.print(",");Serial.print(avgDown);Serial.print(",");Serial.print(avgLeft);Serial.print(",");Serial.println(avgRight);

//int text[] = {topleft,topright,downleft,downright,avgTop,avgDown,avgLeft,avgRight};
//radio.write(&text, sizeof(text));

  if (topleft > topright) 
  {
    diff=topleft-topright;
    if (diff>30)
    {
      digitalWrite(enable,LOW);
      stepperRotate_A(-2, 5);
      digitalWrite(enable,HIGH);
      }
      else if (diff<=30 && diff>10)
      {
        digitalWrite(enable,LOW);
        stepperRotate_A(-1, 5);
        digitalWrite(enable,HIGH);
        }
        else
        {
          digitalWrite(enable,HIGH);
          }
   //stepperRotate_A(-1, 5);
  }
  if (downleft > downright) 
  {
    diff=downleft-downright;
    if (diff>30)
    {
      digitalWrite(enable,LOW);
      stepperRotate_A(-2, 5);
      digitalWrite(enable,HIGH);
      }
      else if (diff<=30 && diff>10)
      {
        digitalWrite(enable,LOW);
        stepperRotate_A(-1, 5);
        digitalWrite(enable,HIGH);
        }
        else
        {
          digitalWrite(enable,HIGH);
          }
   //stepperRotate_A(-1, 5);
  }
  if (topleft < topright) 
  {
    diff=topright-topleft;
    if (diff>30)
    {
      digitalWrite(enable,LOW);
      stepperRotate_A(2, 5);
      digitalWrite(enable,HIGH);
      }
      else if (diff<=30 && diff>10)
      {
        digitalWrite(enable,LOW);
        stepperRotate_A(1, 5);
        digitalWrite(enable,HIGH);
        }
        else
        {
          digitalWrite(enable,HIGH);
          }
   //stepperRotate_A(1, 5);
  }
  if (downleft < downright) 
  {
    diff=downright-downleft;
    if (diff>30)
    {
      digitalWrite(enable,LOW);
      stepperRotate_A(2, 5);
      digitalWrite(enable,HIGH);
      }
      else if (diff<=30 && diff>10)
      {
        digitalWrite(enable,LOW);
        stepperRotate_A(1, 5);
        digitalWrite(enable,HIGH);
        }
        else
        {
          digitalWrite(enable,HIGH);
          }
   //stepperRotate_A(1, 5); 
  }

}


void stepperRotate_A(float rotation, float rpm) 
{
  if (rotation > 0) 
  {
    digitalWrite(dirPin, HIGH);//anti-clock
  }
  else 
  {
    digitalWrite(dirPin, LOW);//clock
    rotation = rotation * -1;
  }

  // first figure out how many steps in one rotation, so a motor with 1.8deg per step, equals 360/1.8 = 200 steps/rotation
  // then if you're doing half step, that will double, so divide by 0.5, gives 400steps per rotation
  float stepsPerRotation = (360.00 / motorAngle) / stepSize;

  //now we have the steps per rotation, multiply by the rotations for this command to get total steps
  float totalSteps = rotation * stepsPerRotation;
  //Serial.println(totalSteps); // debug

  //tricky part here - what is the ON/OFF time of the step pin to get the desired RPM?
  // First, what is the seconds per revolution? then we can figure out seconds per step
  // RPM (rotation per minute) needs to be converted to MPR, so 1/RPM, then seconds per rotation is 60seconds/RPM
  // that gives us Seconds per Rotation, but how many seconds per step? well, we just divide that by the number of steps per rotation
  // so now we're at 60/RPM/stepsPerRotation
  // this is seconds, but we're going to use microSeconds, so let's multiply by 1 Million (1E6)
  // then, we want a 50% duty cycle, half time ON, half time OFF, so divide this value by 2, then we end up with:
  unsigned long stepPeriodmicroSec = ((60.0000 / (rpm * stepsPerRotation)) * 1E6 / 2.0000) - 5;

  //what's up with the -5 at the end?  well, in the for loop, we have to compensate for the i++ and i<x check, so 5us is subracted to speed it up a little

  //Serial.println(stepPeriodmicroSec);

  for (unsigned long i = 0; i < totalSteps; i++) 
  {
    PORTD |= (1 << 3);
    delayMicroseconds(stepPeriodmicroSec);
    PORTD &= ~(1 << 3);
    delayMicroseconds(stepPeriodmicroSec);
  }

}
