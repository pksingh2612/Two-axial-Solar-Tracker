const int stepPin = 2;//only works on this pin right now
const int dirPin = 3;

const int actPin = 4;//not used

const float motorAngle = 1.8;
const float stepSize = 1;//0.5;//0.03125;//full=1, half=0.5, quarter=0.25, etc...


void stepperRotate(float rotation, float rpm);

void setup() 
{
  // put your setup code here, to run once:
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  //pinMode(actPin, OUTPUT);  hooked to VCC, so no Arduino control
  Serial.begin(9600);
}

void loop() 
{
  // put your main code here, to run repeatedly:
  // simple rotation forward then backward:
  stepperRotate(1,100);//rotations, RPM
  delay(5000);
  stepperRotate(-2,100);//rotations, RPM
  delay(5000);
}

void stepperRotate(float rotation, float rpm) 
{
  if (rotation > 0) 
  {
    digitalWrite(dirPin, HIGH);
  }
  else 
  {
    digitalWrite(dirPin, LOW);
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

  for (unsigned long i = 0; i < totalSteps; i++) {
    PORTD |= (1 << 2);
    delayMicroseconds(stepPeriodmicroSec);
    PORTD &= ~(1 << 2);
    delayMicroseconds(stepPeriodmicroSec);
  }

}
