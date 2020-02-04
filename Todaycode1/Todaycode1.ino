// countes are used to stop the setup going beyond the limit.
// the limit band for 360 motor is 0 to 30. The system below this value going to be halt(or stop).
// the limit band for axial motor(linear motor) is 0 to 50. The system below this value going to be halt(or stop). 
// header file
#include <EEPROM.h>

// motor A (360)
#define stepPin1 3 // this is the step pin that make step movement of 360 motor. 
#define dirPin1 2  // this is the dir pin that decide the clock and anticlock movement of 360 motor. 
#define enable1 6  // this enable pin decide the enable and disable event of 360 motor.
int counter1; // this is the counter that record the step movement when 360 motors moves from 0 to 30. 
int flag1=0;   
int d1(int dif,int y); 
void stepperRotate1(float rotation, float rpm);

// motor B (axial)
#define stepPin2 5 // this is the step pin that make step movement of axial motor.
#define dirPin2 4  // this is the dir pin that decide the clock and anticlock movement of axial motor.
#define enable2 7  // this enable pin decide the enable and disable event of axial motor.
int counter2; // this is the counter that record the step movement when axial motors moves from 0 to 50. 
int flag2=0;
int d2(int dif,int y);
void stepperRotate2(float rotation, float rpm);

const float motorAngle = 1.8;
const float stepSize = 1;//full=1, half=0.5, quarter=0.25, etc...

int o;

void setup() 
{
  // motorA setup (360) 
  pinMode(stepPin1, OUTPUT);
  pinMode(dirPin1, OUTPUT);
  pinMode(enable1,OUTPUT);

  // motorB setup (axial)
  pinMode(stepPin2, OUTPUT);
  pinMode(dirPin2, OUTPUT);
  pinMode(enable2,OUTPUT);
  
  // begin serial monitor
  Serial.begin(9600);
 
  //when power disconnect we are going to store the last existing values of counter1 and counter2 
  //for respective 360 and axial motors  
    
  counter1 = EEPROM.read(0);//assign counter the value of address 0
  //write a 0 to address 0. This allows for consecutive resets to reset the counter
  EEPROM.write(0,0);
  
  counter2 = EEPROM.read(1);//assign counter the value of address 1
  //write a 0 to address 0. This allows for consecutive resets to reset the counter
  EEPROM.write(1,5);  
}

void loop() 
{
  int topleft = analogRead(A0); // it record the top left of photodiode value
  int topright = analogRead(A1);// it record the top right of photodiode value
  int downleft = analogRead(A2);// it record the down left of photodiode value
  int downright = analogRead(A3);// it record the down right of photodiode value

  // avgLeft and avgRight value is used for 360 motor step movement(either in clock-wise or anti-clock wise).  
  int avgLeft=(topleft+downleft)/2;
  int avgRight=(topright+downright)/2;
  
  // avgTop and avgDown value is used for axial motor step movement(either in clock-wise or anti-clock wise).
  int avgTop=(topleft+topright)/2;
  int avgDown=(downleft+downright)/2;

  // values shown on serial monitor
  Serial.print("TL");Serial.print(",");Serial.print("TR");Serial.print(",");Serial.print("DL");Serial.print(",");Serial.print("DR");Serial.print(",");Serial.print("AT");Serial.print(",");Serial.print("AD");Serial.print(",");Serial.print("AL");Serial.print(",");Serial.print("AR");Serial.print(",");Serial.print("Counter1");Serial.print(",");Serial.println("Counter2");
  Serial.print(topleft);Serial.print(",");Serial.print(topright);Serial.print(",");Serial.print(downleft);Serial.print(",");Serial.print(downright);Serial.print(",");Serial.print(avgTop);Serial.print(",");Serial.print(avgDown);Serial.print(",");Serial.print(avgLeft);Serial.print(",");Serial.print(avgRight);Serial.print(",");Serial.print(counter1);Serial.print(",");Serial.println(counter2);

  // when sunrise we observe that all four photodiode value are going below 100 and 
  // hence the below if condition is become true.
  if (topleft<100 && topright<100 && downleft<100 && downright<100)
  {
      o=1;
      // the below four possible condition is for the movement of whole setup according to sun movement. 
      if (avgLeft > avgRight) 
      {
        //if the avgLeft > avgRight then setup moves in anticlock-wise direction. 
        EEPROM.write(0, d1(avgLeft - avgRight,-1)); //write counter1 to address 0
      }
      
      if (avgLeft < avgRight) 
      {
        //if avgLeft < avgRight then setup moves in clock-wise direction. 
        EEPROM.write(0, d1(avgRight-avgLeft,1)); //write counter1 to address 0
      }
      
      if (avgTop > avgDown) 
      {
         //if avgTop > avgDown then setup moves down.
         // act. up ---> setup down 
        EEPROM.write(1, d2(avgTop - avgDown,1)); //write counter to address 1
      }
      
      if (avgTop < avgDown) 
      {
        //if avgTop < avgDown then setup moves up.
        // act. down ---> setup up
        EEPROM.write(1, d2(avgDown-avgTop,-1)); //write counter to address 1
      }

      // above this band the 360 motor going to stop 
      if (counter1>=30)
      {
        digitalWrite(enable1,HIGH);
        counter1=30;
        flag1=1;
        EEPROM.write(0, counter1); //write counter to address 0
      }

      // below this band the 360 motor going to stop
      if (counter1<0)
      {
        digitalWrite(enable1,HIGH);
        counter1=0;
        flag1=1;
        EEPROM.write(0,counter1); //write counter to address 0    
      }

      // above this band the axial motor going to stop 
      if (counter2>=50)
      {
      digitalWrite(enable2,HIGH);
      counter2=50;
      flag2=1;
      EEPROM.write(1, counter2); //write counter to address 1
      }

      // below this band the axial motor going to stop 
      if (counter2<0)
      {
      digitalWrite(enable2,HIGH);
      counter2=0;
      flag2=1;
      EEPROM.write(1,counter2); //write counter to address 1
      }
  }
  else
  {
    // the 360 and axial motor is diable.
    digitalWrite(enable1,HIGH);
    digitalWrite(enable2,HIGH);
    delay(1000);

    if(o==1)
    {
    if (counter1==30)
    {
      flag1=0;
      digitalWrite(enable1,HIGH);
      for (int y=1;y<=3;y++)
      {
      digitalWrite(enable1,LOW);
      stepperRotate1(-10, 5);//30
      digitalWrite(enable1,HIGH);
      delay(20000);
      counter1-=10;
      EEPROM.write(0, counter1); //write counter to address 0
      }
    }
    else
    {
      flag1=0;
      digitalWrite(enable1,HIGH);
      counter1 = EEPROM.read(0);
      digitalWrite(enable1,LOW);
      stepperRotate1(-counter1, 5);
      digitalWrite(enable1,HIGH);
      counter1=0;
      EEPROM.write(0, counter1); //write counter to address 0
    }
    if (counter2==50)
    {
      flag2=0;
      digitalWrite(enable2,HIGH);
      for (int y=1;y<=5;y++)
      {
      digitalWrite(enable2,LOW);
      stepperRotate2(10, 5);//50 acc. up
      digitalWrite(enable2,HIGH);
      delay(20000);
      counter2-=10;
      EEPROM.write(1, counter2); //write counter to address 1   
      }
    }
    else
    {
       flag2=0;
       digitalWrite(enable2,HIGH);
       counter2 = EEPROM.read(1);
       digitalWrite(enable2,LOW);
       stepperRotate2(counter2, 5);
       digitalWrite(enable2,HIGH);
       counter2=5;
       EEPROM.write(1, counter2); //write counter to address 0
     }
     o+=1;
     }
  }
}

int d1(int dif,int y)
{
  //if dif is between 1024 and 60 then step movement of 360 motor is increment by 2 rotation.
  if (dif>60 && flag1==0)
  {
      digitalWrite(enable1,LOW);
      stepperRotate1(2*y, 5);
      digitalWrite(enable1,HIGH);
      if (y<0)
      {counter1-=2;}
      else 
      {counter1+=2;}
  }
  //if dif is [60,2] then step movement of 360 motor is increment by 1 rotation.
  else if (dif<=60 && dif>=2 && flag1==0)
  {
        digitalWrite(enable1,LOW);
        stepperRotate1(1*y, 5);
        digitalWrite(enable1,HIGH);
        if (y<0)
          {counter1-=1;}
        else 
          {counter1+=1;}
  }
  // otherwise the 360 motor going to be disable.
  else
    {
       digitalWrite(enable1,HIGH);
       delay(2000);
    }
return counter1;
}

int d2(int dif,int y)
{
  //if dif is between 1024 and 60 then step movement of axial motor is increment by 2 rotation.
  if (dif>60 && flag2==0)
    {
      digitalWrite(enable2,LOW);
      stepperRotate2(2*y, 5);
      digitalWrite(enable2,HIGH);
      if (y<0)
      {counter2-=2;}
      else 
      {counter2+=2;}
    }
  //if dif is [60,2] then step movement of axial motor is increment by 1 rotation.
  else if (dif<=60 && dif>=2 && flag2==0)
    {
        digitalWrite(enable2,LOW);
        stepperRotate2(1*y, 5);
        digitalWrite(enable2,HIGH);
        if (y<0)
          {counter2-=1;}
        else 
          {counter2+=1;}
    }
    // otherwise the axial motor going to be disable.
  else
    {
       digitalWrite(enable2,HIGH);
       delay(2000);
    }
return counter2;
}

// This is function for step movement as per rotation of 360 motor
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

// This is function for step movement as per rotation of axial motor
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
