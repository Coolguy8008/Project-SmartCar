#include <IRremote.h>
#include <Arduino.h>
#include <IRfunctions.h>
#include <behaviours.h>

typedef void (*FuncPtr)();
int actionTime = random(2,5);
unsigned long lastCommandTime;            // Record the time of the last received command
const unsigned long commandTimeout = 100; // Set the timeout period (milliseconds)
uint32_t last_decode = 0;                 // Variable to store the previously decoded raw data
uint32_t current_decode = 0;              // Variable to store the currently decoded raw data

// IDLE SETUP
void action0()
{
 Serial.println("0");
  myCar.Move(Forward, SPEED);
  delay(actionTime);
  myCar.Move(Stop, 0);
  Serial.println("0 a");
}
void action1()
{
  myCar.Move(Backward, SPEED);
 // delay(actionTime);
  //myCar.Move(Stop, 0);
  Serial.println("1");
}
void action2() { Serial.println("2"); }
void action3() { Serial.println("3"); }
void action4() { Serial.println("4"); }
void action5() { Serial.println("5"); }
void action6() { Serial.println("6"); }
void action7() { Serial.println("7"); }

FuncPtr actions[20] = {
    action0, action1, action2, action3, action4, action5, action6, action7};

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
  randomSeed(analogRead(0));
  pinMode(leftLEDPin, OUTPUT);
  pinMode(rightLEDPin, OUTPUT);
  pinMode(IrLeft, INPUT);
  pinMode(IrRight, INPUT);
}

void reset()
{
  leftSide = 0;
  rightSide = 0;
  target = 0;
  facingTarget = false;
  UT_distance = 0;
  recieveInstruction = true;
  followState = false;
  manual = false;
  leftIR = 0;
  rightIR = 0;
  SPEED = 200;
  myServo.write(90);
  myCar.Move(Stop, 0);
}

// IR BUTTON FUNCTIONS

void handleIRrecieve()
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
      if (recieveInstruction)
      {
        arrowUp();
      }
      break;

    case 0xEA15FF00:
      if (recieveInstruction)
      {
        arrowDown();
      }
      break;

    case 0xBB44FF00:
      if (recieveInstruction)
      {
        arrowLeft();
      }
      break;
    case 0xBC43FF00:
      if (recieveInstruction)
      {
        arrowRight();
      }
      break;

    case 0xE916FF00:
      if (recieveInstruction)
      {
        but1();
      }
      break;

    case 0xF20DFF00:
      if (recieveInstruction)
      {
        but3();
      }
      break;

    case 0xE619FF00:
      if (recieveInstruction)
      {
        but2();
      }
      break;

    case 0xBD42FF00:
      if (recieveInstruction)
      {
        findClosest();
      }
      break;

    case 0xB54AFF00:
      if (recieveInstruction)
      {
        toggleFollow();
      }
      break;

    case 0xBF40FF00:
      if (recieveInstruction)
      {
        toggleManual();
      }
      break;

    case 0xF708FF00:
      if (recieveInstruction)
      {
        speedDown();
      }
      break;

    case 0xA55AFF00:
      if (recieveInstruction)
      {
        speedUp();
      }
      break;
    case 0xAD52FF00:
      reset();
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

void updateSensors()
{
  rightIR = analogRead(IrLeft);
  leftIR = analogRead(IrRight);
  UT_distance = myUltrasonic.Ranging();
}

void loop()
{
  actionTime = random(2, 5);
  Serial.println(actionTime);
  int choice = random(0, 7); // Generates number 0 to 19
  actions[choice]();         // Call selected function
  updateSensors();
  checkDistance();
  handleIRrecieve();
  ledController();
  delay(1000);
}
