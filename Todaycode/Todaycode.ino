#define stepPin1 3
#define dirPin1 2
#define enable1 6

#define stepPin2 5
#define dirPin2 4
#define enable2 7

const float motorAngle = 1.8;
const float stepSize = 1;//full=1, half=0.5, quarter=0.25, etc...

void d1(int dif,int y);
void stepperRotate1(float rotation, float rpm);

void d2(int dif,int y);
void stepperRotate2(float rotation, float rpm);

void setup() 
{
  pinMode(stepPin1, OUTPUT);
  pinMode(dirPin1, OUTPUT);
  pinMode(enable1,OUTPUT);

  pinMode(stepPin2, OUTPUT);
  pinMode(dirPin2, OUTPUT);
  pinMode(enable2,OUTPUT);
  
  Serial.begin(9600);
}

void loop() 
{
  int topleft = analogRead(A0);
  int topright = analogRead(A1);
  int downleft = analogRead(A2);
  int downright = analogRead(A3);
  
  int avgLeft=(topleft+downleft)/2;
  int avgRight=(topright+downright)/2;
  int avgTop=(topleft+topright)/2;
  int avgDown=(downleft+downright)/2;

  Serial.print("TL");Serial.print(",");Serial.print("TR");Serial.print(",");Serial.print("DL");Serial.print(",");Serial.print("DR");Serial.print(",");Serial.print("AT");Serial.print(",");Serial.print("AD");Serial.print(",");Serial.print("AL");Serial.print(",");Serial.println("AR");
  Serial.print(topleft);Serial.print(",");Serial.print(topright);Serial.print(",");Serial.print(downleft);Serial.print(",");Serial.print(downright);Serial.print(",");Serial.print(avgTop);Serial.print(",");Serial.print(avgDown);Serial.print(",");Serial.print(avgLeft);Serial.print(",");Serial.println(avgRight);

    if (avgLeft > avgRight) 
    {
      d1(avgLeft - avgRight,-1);
    }
  
    if (avgLeft < avgRight) 
    {
      d1(avgRight-avgLeft,1);
    }
   
    if (avgTop > avgDown) 
    {
      d2(avgTop - avgDown,1);
    }
    
    if (avgTop < avgDown) 
    {
      d2(avgDown - avgTop,-1);
    }
}

void d1(int dif,int y)
{
  if (dif>=2)
    {
        digitalWrite(enable1,LOW);
        stepperRotate1(1*y, 5);
        digitalWrite(enable1,HIGH);
    }
  else
    {
       digitalWrite(enable1,HIGH);
       delay(2000);
    }
}

void d2(int dif,int y)
{
 if (dif>=2)
    {
        digitalWrite(enable2,LOW);
        stepperRotate2(1*y, 5);
        digitalWrite(enable2,HIGH);
    }
  else
    {
       digitalWrite(enable2,HIGH);
       delay(2000);
    }
}

void stepperRotate1(float rotation, float rpm) 
{
  if (rotation > 0) 
  {
    digitalWrite(dirPin1, HIGH);//anti-clock
  }
  else 
  {
    digitalWrite(dirPin1, LOW);//clock
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

void stepperRotate2(float rotation, float rpm) 
{
  if (rotation > 0) 
  {
    digitalWrite(dirPin2, HIGH);//anti-clock
  }
  else 
  {
    digitalWrite(dirPin2, LOW);//clock
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
    PORTD |= (1 << 5);
    delayMicroseconds(stepPeriodmicroSec);
    PORTD &= ~(1 << 5);
    delayMicroseconds(stepPeriodmicroSec);
  }
}
