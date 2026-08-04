#include <Arduino.h>
#include <vehicle.h>
#include <ultrasonic.h>
#include <ESP32Servo.h>

#define leftLEDPin 2
#define rightLEDPin 12
#define buzzerPin 33
#define servoPin 25
#define sonarPin 14
#define echoPin 15
#define antiClockwise Contrarotate
vehicle myCar;
ultrasonic myUltrasonic;
Servo myServo;

int UT_distance = 0;
// MOTOR TERMS (USE INSIDE OF "myCar.Move()") speed is out of 255
//   Forward, [speed] & Backward [speed]
//   Clockwise, [speed] & antiClockwise, [speed]
//   Move_Left, [speed] & Move_Right, [speed]
//   Stop, [speed]

void setup()
{
  Serial.begin(115200);
  myCar.Init();
  myServo.attach(servoPin);
  myServo.write(90);
  myUltrasonic.Init(13, 14);
  pinMode(leftLEDPin, OUTPUT);
  pinMode(rightLEDPin, OUTPUT);
}

void checkDistance()
{
  if (UT_distance > 60)
  {
    myCar.Move(Forward, 200);
    tone(buzzerPin, 0);
  }
  else
  {
    if (UT_distance < 40)
    {
      myCar.Move(Backward, 200);
      tone(buzzerPin, 0);
    }
    else
    {
      myCar.Move(Stop, 0);
      tone(buzzerPin, 262);
    }
  }
}

void loop()
{
  UT_distance = myUltrasonic.Ranging();
  checkDistance();
  myServo.write(90);
  // Serial.print(UT_distance);
  // Serial.println("cm");
  delay(250);
}
