#include <IRremote.h>
#include <Arduino.h>
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

// FACE OBJECT VARIABLES
int leftSide;
int rightSide;
int target;
bool facingTarget = false;

IRrecv myIRrecv(IRpin);
ultrasonic myUltrasonic;
Servo myServo;

unsigned long lastCommandTime;            // Record the time of the last received command
const unsigned long commandTimeout = 100; // Set the timeout period (milliseconds)
uint32_t last_decode = 0;                 // Variable to store the previously decoded raw data
uint32_t current_decode = 0;              // Variable to store the currently decoded raw data

int UT_distance = 0;
bool recieveInstruction = true;
// MOTOR TERMS (USE INSIDE OF "myCar.Move()") speed is out of 255
//   Forward, [speed] & Backward [speed]
//   Clockwise, [speed] & antiClockwise, [speed]
//   Move_Left, [speed] & Move_Right, [speed]
//   Stop, [speed]

// BUZZER CONTROL
// tone(buzzerPin, [value])

void setup()
{
  Serial.begin(115200);
  myIRrecv.enableIRIn();
  myCar.Init();
  myServo.attach(servoPin);
  myServo.write(90);
  myUltrasonic.Init(13, 14);
  pinMode(leftLEDPin, OUTPUT);
  pinMode(rightLEDPin, OUTPUT);
}
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
// IR BUTTON FUNCTIONS

void handleIRrecieve()
{
  if (recieveInstruction)
  {
    if (myIRrecv.decode())
    {                             // Check if an IR signal is received
      lastCommandTime = millis(); // Update the last command time
      current_decode = myIRrecv.decodedIRData.decodedRawData;
      if (myIRrecv.decodedIRData.flags)
      { // Check if it's a repeated IR code
        current_decode = last_decode;
        // Set current decode as the previous one
      }
      Serial.print(current_decode, HEX);
      Serial.println("");
      switch (current_decode)
      {
      case 0xB946FF00:
        arrowUp();
        break;

      case 0xEA15FF00:
        arrowDown();
        break;

      case 0xBB44FF00:
        arrowLeft();
        break;
      case 0xBC43FF00:
        arrowRight();
        break;

      case 0xE916FF00:
        but1();
        break;

      case 0xF20DFF00:
        but3();
        break;

      case 0xE619FF00:
        but2();
        break;

      case 0xBD42FF00:
        findClosest();
        break;

      case 0xB54AFF00:
        toggleFollow();
        break;

      case 0xBF40FF00:
        toggleManual();
        break;

      case 0xF708FF00:
        speedUp();
        break;

      case 0xA55AFF00:
        speedDown();
        break;
      }
      last_decode = current_decode;
      // Update the stored previous decodedRawData
      myIRrecv.resume(); // Wait for the next IR signal
    }
    if ((millis() - lastCommandTime > commandTimeout) && (manual))
    {
      myCar.Move(Stop, 0);
      // If no new IR signal within 100 milliseconds, stop the smart car
    }
  }
}
void ledController()
{
  digitalWrite(leftLEDPin, manual);
  digitalWrite(rightLEDPin, followState);
}

void checkDistance()
{
  if (followState == true)
  {
    if (UT_distance > 60)
    {
      myCar.Move(Forward, SPEED);
      // tone(buzzerPin, 0);
    }
    else
    {
      if (UT_distance < 40)
      {
        myCar.Move(Backward, SPEED);
        // tone(buzzerPin, 0);
      }
      else
      {
        myCar.Move(Stop, 0);
        //  tone(buzzerPin, 262);
      }
    }
  }
}

void loop()
{
  UT_distance = myUltrasonic.Ranging();
  checkDistance();
  handleIRrecieve();
  ledController();
  Serial.println(SPEED);
  delay(250);
}
