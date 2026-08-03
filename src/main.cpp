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

int UT_ditance = 0;
// MOTOR TERMS (USE INSIDE OF "myCar.Move()")!!
//   Forward & Backward
//   Clockwise & antiClockwise
//   Move_Left & Move_Right
//   Stop

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

void loop()
{
  UT_ditance = myUltrasonic.Ranging();
  Serial.print(UT_ditance);
  Serial.println("cm");
  delay(1000);
}
