#include <vehicle.h>
#include <ultrasonic.h>
#include <ESP32Servo.h>
#include <IRfunctions.h>

#define IRpin 4
#define leftLEDPin 2
#define rightLEDPin 12
#define buzzerPin 33
#define servoPin 25
#define sonarPin 14
#define echoPin 15
#define IrLeft 35
#define IrRight 39
#define antiClockwise Contrarotate
#define buzzerPin 33

vehicle myCar;

int SPEED = 200;
bool followState = false;
bool manual = false;
IRrecv myIRrecv(IRpin);
ultrasonic myUltrasonic;
Servo myServo;

// FACE OBJECT VARIABLES
int leftSide = 0;
int rightSide = 0;
int target;
bool facingTarget = false;
int UT_distance = 0;
bool recieveInstruction = true;
float leftIR = 0;
float rightIR = 0;
void findLeft()
{
  while (facingTarget == false)
  {

    UT_distance = myUltrasonic.Ranging();
    myCar.Move(antiClockwise, 200);
    Serial.println(UT_distance);
    if (target - 3 <= UT_distance && target + 3 >= UT_distance)
    {
      Serial.println("FOUND");
      facingTarget = true;
    }
    delay(100);
  }
  // delay(500);
  myCar.Move(Stop, 0);
}

void findRight()
{
  while (facingTarget == false)
  {
    UT_distance = myUltrasonic.Ranging();
    myCar.Move(Clockwise, 200);
    Serial.println(UT_distance);
    if (target - 3 <= UT_distance && target + 3 >= UT_distance)
    {
      Serial.println("FOUND");
      facingTarget = true;
    }
    delay(100);
  }
  // delay(500);
  myCar.Move(Stop, 0);
}

void findClosest()
{
  if ((followState == false) & (manual == false))
  {
    // FIND RIGHT DISTANCE
    recieveInstruction = false;
    myServo.write(0);
    delay(300);
    rightSide = myUltrasonic.Ranging();
    Serial.print(rightSide);
    Serial.println(" RIGHT");
    // FIND LEFT SIDE
    myServo.write(180);
    delay(400);
    leftSide = myUltrasonic.Ranging();
    Serial.print(leftSide);
    Serial.println(" LEFT");
    // FIND LOWEST
    target = min(leftSide, rightSide);
    myServo.write(90);
    delay(400);
    Serial.print("target: ");
    Serial.println(target);
    delay(500);
    // CHOOSE FACE SIDE
    if (target < 100)
    {
      if (target == leftSide)
      {
        findLeft();
      }
      else
      {
        findRight();
      }
    }
    // RESET
    leftSide = 0;
    rightSide = 0;
    recieveInstruction = true;
    facingTarget = false;
  }
}
