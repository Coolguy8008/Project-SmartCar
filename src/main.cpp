#include <IRremote.h>
#include <Arduino.h>
#include <vehicle.h>
#include <ultrasonic.h>
#include <ESP32Servo.h>
#include <idleFunctions.h>
#include <Wire.h>
#define IRpin 4
#define buzzerPin 33
#define servoPin 25
#define sonarPin 14
#define echoPin 15
#define Left_sensor 35   // declare the pin of left tracking sensor
#define Middle_sensor 36 // declare the pin of middle tracking sensor
#define Right_sensor 39  // declare the pin of right tracking sensor


int Speed = 250;
int RotateSpeed = 150;
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
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  myServo.attach(servoPin);
  myServo.write(90);
  myUltrasonic.Init(13, 14);
  pinMode(leftLEDPin, OUTPUT);
  pinMode(rightLEDPin, OUTPUT);
  pinMode(Left_sensor, INPUT);
  pinMode(Middle_sensor, INPUT);
  pinMode(Right_sensor, INPUT);
  pinMode(crashPin, INPUT);
}

void ledController()
{
  digitalWrite(leftLEDPin, manual);
  digitalWrite(rightLEDPin, followState);
}
void lineFollow()
{
  if (followingLine)
  {
    Left_Tra_Value = analogRead(Left_sensor);
    Middle_Tra_Value = analogRead(Middle_sensor);
    Right_Tra_Value = analogRead(Right_sensor);
    delay(5);
    if (Left_Tra_Value < Black_Line && Middle_Tra_Value >= Black_Line && Right_Tra_Value < Black_Line)
    {
      myCar.Move(Forward, Speed); // Smart car forward
    }
    if (Left_Tra_Value < Black_Line && Middle_Tra_Value >= Black_Line && Right_Tra_Value >= Black_Line)
    {
      myCar.Move(Forward, 180);
    }
    if (Left_Tra_Value >= Black_Line && Middle_Tra_Value >= Black_Line && Right_Tra_Value < Black_Line)
    {
      myCar.Move(Forward, 180);
    }
    else if (Left_Tra_Value >= Black_Line && Middle_Tra_Value < Black_Line && Right_Tra_Value < Black_Line)
    {
      myCar.Move(Contrarotate, 220);
    }
    else if (Left_Tra_Value < Black_Line && Middle_Tra_Value < Black_Line && Right_Tra_Value >= Black_Line)
    {
      myCar.Move(Clockwise, 220);
    }
    else if (Left_Tra_Value >= Black_Line && Middle_Tra_Value >= Black_Line &&
             Right_Tra_Value >= Black_Line)
    {
      myCar.Move(Forward, 180);
    }
  }
}

void loop()
{
  UT_distance = myUltrasonic.Ranging();
  lineFollow();
  checkDistance();
  handleIRrecieve();
  ledController();
  randBehaviour();
  delay(refreshSpeed);
}