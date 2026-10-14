#include <IRremote.h>
#include <Arduino.h>
#include <vehicle.h>
#include <ultrasonic.h>
#include <ESP32Servo.h>
#include <idleFunctions.h>

#define IRpin 4
#define buzzerPin 33
#define servoPin 25
#define sonarPin 14
#define echoPin 15
#define Left_sensor 35   // declare the pin of left tracking sensor
#define Middle_sensor 36 // declare the pin of middle tracking sensor
#define Right_sensor 39  // declare the pin of right tracking sensor

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
  randomSeed(analogRead(A0));
  myIRrecv.enableIRIn();
  myCar.Init();
  myServo.attach(servoPin);
  myServo.write(90);
  myUltrasonic.Init(13, 14);
  pinMode(leftLEDPin, OUTPUT);
  pinMode(rightLEDPin, OUTPUT);
  pinMode(Left_sensor, INPUT);
  pinMode(Middle_sensor, INPUT);
  pinMode(Right_sensor, INPUT);
}

void ledController()
{
  digitalWrite(leftLEDPin, manual);
  digitalWrite(rightLEDPin, followState);
}

void loop()
{
  UT_distance = myUltrasonic.Ranging();
  checkDistance();
  handleIRrecieve();
  ledController();
  randBehaviour();
  delay(refreshSpeed);
}