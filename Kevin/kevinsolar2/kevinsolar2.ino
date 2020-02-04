// header files
#include <SPI.h>  
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>

//nrf configuration
RF24 radio(9, 10); // CE, CSN
const byte address[6] = "00001";


// motor configuration
const float motorAngle = 1.8; // specify the motor angle (i.e. 1.8 degree)
const float stepSize = 0.5;//full=1, half=0.5, quarter=0.25, etc... // here we select microstepping (i.e. 0.25)

// For Motor A - Linear motor
#define dirPin_A 2   // to change the direction of motorA (Linear motor)
#define stepPin_A 3 // to move the motorA by certain steps
//#define enable_A 6 // to enable the motorA ON or OFF

int countrotation1=0; // counter for motor A

void stepperRotate_A(float rotation, float rpm);

// For Motor B - Axial motor
//#define dirPin_B 4   // to change the direction of motor B(axial motor)
//#define stepPin_B 5 // to move the motor B by certain steps
//#define enable_B 7 // to enable the motorB ON or OFF
//int countrotation=0;  // counter for motor B
void stepperRotate_B(float rotation, float rpm);

void setup() 
{   
    // nrf module setup
    radio.begin();
    radio.openWritingPipe(address);
    radio.setPALevel(RF24_PA_MIN);
    radio.stopListening();

    // setup for motorA
    pinMode(stepPin_A, OUTPUT);
    pinMode(dirPin_A, OUTPUT);
  //  pinMode(enable_A,OUTPUT);
  
    // setup for motorB  
//  pinMode(stepPin_B, OUTPUT);
//  pinMode(dirPin_B, OUTPUT);
//  pinMode(enable_B,OUTPUT);

  Serial.begin(9600);
}

void loop() 
{
  int diff=0;
  
  // Value taken from 4 photo-diode placed as shown in image
  int topleft = analogRead(A0);
  int topright = analogRead(A1);
  int downleft = analogRead(A2);
  int downright = analogRead(A3);

  //int avgLeft=(topleft+downleft)/2;
  //int avgRight=(topright+downright)/2;
  //int avgTop=(topleft+topright)/2;
  //int avgDown=(downleft+downright)/2;

  Serial.print("TL");Serial.print(",");Serial.print("TR");Serial.print(",");Serial.print("DL");Serial.print(",");Serial.println("DR");
  Serial.print(topleft);Serial.print(",");Serial.print(topright);Serial.print(",");Serial.print(downleft);Serial.print(",");Serial.println(downright);
  //Serial.print(avgLeft);Serial.print(",");Serial.print(avgRight);Serial.print(",");Serial.print(avgTop);Serial.print(",");Serial.println(avgDown);

   int text[] = {topleft,topright,downleft,downright};
   radio.write(&text, sizeof(text));

  if (topleft>topright ) //avgLeft>avgRight
  {
  //   diff=topleft-topright;
 //    if(diff>30)
  //   {
  //     digitalWrite(enable_A,LOW);
       stepperRotate_A(-3, 5);//rotations, RPM // clock-wise
  //     digitalWrite(enable_A,HIGH);
       //countrotation1+=3;
  //   }     
  //   if(diff<=30 && diff>0)
  //   {
  //     digitalWrite(enable_A,LOW);
       stepperRotate_A(-1, 5);//rotations, RPM //clockwise
  //     digitalWrite(enable_A,HIGH);
       //countrotation1+=1;
  //   }
  //   if(diff==0)
  //   {
  //     digitalWrite(enable_A,HIGH);
  //   }
    
    }


  if (topleft<topright) //avgLeft<avgRight
    {
  //   diff=topright-topleft;
  //   if(diff>30)
  //   {
    //   digitalWrite(enable_A,LOW);
       stepperRotate_A(3, 5);//rotations, RPM //anti-clock
    //   digitalWrite(enable_A,HIGH);
       //countrotation1-=3;
 //    }     
 //    if(diff<=30 && diff>0)
 //    {
     //  digitalWrite(enable_A,LOW);
       stepperRotate_A(1, 5);//rotations, RPM //anti-clock
     //  digitalWrite(enable_A,HIGH);
       //countrotation1-=1;
 //    }
    // if(diff==0)
    // {
    //   digitalWrite(enable_A,HIGH);
    // }
    }

//   if (countrotation1>=12)
//   {
//     digitalWrite(enable_A,HIGH);
//     digitalWrite(enable_A,LOW);
//     stepperRotate_B(12, 5);//rotations, RPM // /anti-clock-wise -- move right
//     digitalWrite(enable_A,HIGH);
//     countrotation-=12;
//   }
//   if (countrotation1<=-12)
//   {
//     digitalWrite(enable_A,HIGH);
//     digitalWrite(enable_A,LOW);
//     stepperRotate_B(-3, 5);//rotations, RPM // /clock-wise -- move left
//     digitalWrite(enable_A,HIGH);
//     countrotation+=3;
//   }

 //   if (avgTop> avgDown) 
 //   {
 //    diff=avgTop-avgDown;
 //    if(diff>30)
 //    {
 //      digitalWrite(enable_B,LOW);
 //      stepperRotate_B(-3, 5);//rotations, RPM //clock-wise -- move down
 //      digitalWrite(enable_B,HIGH);
 //      countrotation+=3;
 //    }     
 
 //    if(diff<=30 && diff>0)
 //    {
 //      digitalWrite(enable_B,LOW);
 //      stepperRotate_B(-1, 5);//rotations, RPM //clock-wise -- move down
 //      digitalWrite(enable_B,HIGH);
 //      countrotation+=1;
 //    }
//     if(diff==0)
//     {
//       digitalWrite(enable_B,HIGH); // motor disable
//     }
//    }


//  if (avgTop< avgDown) 
//    {
//     diff=avgDown-avgTop;
//     if(diff>30)
//     {
//       digitalWrite(enable_B,LOW);
//       stepperRotate_B(3, 5);//rotations, RPM // /anti-clock-wise -- move up
//       digitalWrite(enable_B,HIGH);
//       countrotation-=3;
//     }     
 
 //    if(diff<=30 && diff>0)
 //    {
 //      digitalWrite(enable_B,LOW);
 //     stepperRotate_B(3, 5);//rotations, RPM // /anti-clock-wise -- move up
 //      digitalWrite(enable_B,HIGH);
 //      countrotation-=1;
 //    }
     
 //    if(diff==0)
 //    {
 //      digitalWrite(enable_B,HIGH);
 //    }
 //   }

//   if (countrotation>=12)
//   {
//     digitalWrite(enable_B,HIGH);
//     digitalWrite(enable_B,LOW);
//     stepperRotate_B(3, 5);//rotations, RPM // /anti-clock-wise -- move up
//     digitalWrite(enable_B,HIGH);
//     countrotation-=3;
//   }
//   if (countrotation<=-12)
//   {
//     digitalWrite(enable_B,HIGH);
//     digitalWrite(enable_B,LOW);
//     stepperRotate_B(-3, 5);//rotations, RPM // /clock-wise -- move down
//     digitalWrite(enable_B,HIGH);
//     countrotation+=3;
//   }

}

void stepperRotate_A(float rotation, float rpm) 
{
  if (rotation > 0) 
  {
    digitalWrite(dirPin_A, HIGH);//clockwise
  }
  else 
  {
    digitalWrite(dirPin_A, LOW);//anti-clockwise
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


void stepperRotate_B(float rotation, float rpm) 
{
  if (rotation > 0) 
  {
//    digitalWrite(dirPin_B, HIGH);//clockwise
  }
  else 
  {
//    digitalWrite(dirPin_B, LOW);//anti-clockwise
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
    PORTD |= (1 << 5);
    delayMicroseconds(stepPeriodmicroSec);
    PORTD &= ~(1 << 5);
    delayMicroseconds(stepPeriodmicroSec);
  }

}
