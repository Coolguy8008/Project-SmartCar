#include <Arduino.h>
#include <vehicle.h>
#define antiClockwise Contrarotate

vehicle myCar;

int SPEED = 200;
bool followState = false;



void arrowLeft()
{
  myCar.Move(antiClockwise, SPEED);
}

void arrowRight()
{
  myCar.Move(Clockwise, SPEED);
}

void arrowUp()
{
  myCar.Move(Forward, SPEED);
}

void arrowDown()
{
  myCar.Move(Backward, SPEED);
}

void but1()
{
  myCar.Move(Move_Left, SPEED);
}

void but2()
{

}

void but3()
{
  myCar.Move(Move_Right, SPEED);
}

void faceObject()
{

}

void toggleFollow()
{
    followState = !followState;
}