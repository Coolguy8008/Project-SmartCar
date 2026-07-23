#include <Arduino.h>
#include <vehicle.h>
#include <ultrasonic.h>
#include <ESP32Servo.h>

#define leftLEDPin 2
#define rightLEDPin 12
#define antiClockwise Contrarotate
vehicle myCar;


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
  myCar.Move(Move_Right, 255);
  delay(1000);
  digitalWrite(rightLEDPin, HIGH);
  digitalWrite(leftLEDPin, LOW);
  myCar.Move(Move_Left,255);

  delay(1000);
}
