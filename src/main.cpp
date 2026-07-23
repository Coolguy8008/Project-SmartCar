#include <Arduino.h>
#include <vehicle.h>
#include <ultrasonic.h>
#include <ESP32Servo.h>

#define leftLEDPin 2
#define rightLEDPin 12
#define buzzerPin 33
#define antiClockwise Contrarotate
vehicle myCar;

// MOTOR TERMS (USE INSIDE OF "myCar.Move()")!!
//   Forward & Backward
//   Clockwise & antiClockwise
//   Move_Left & Move_Right
//   Stop

void setup()
{
  Serial.begin(115200);
  myCar.Init();
  pinMode(leftLEDPin, OUTPUT);
  pinMode(rightLEDPin, OUTPUT);
}

void loop()
{
  digitalWrite(rightLEDPin, LOW);
  digitalWrite(leftLEDPin, HIGH);
 // myCar.Move(Move_Right, 255);
  tone(buzzerPin, 262);
  delay(1000);
  digitalWrite(rightLEDPin, HIGH);
  digitalWrite(leftLEDPin, LOW);
 // myCar.Move(Move_Left,255);
  noTone(buzzerPin);

  delay(1000);
}
