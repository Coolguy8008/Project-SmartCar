#include <Arduino.h>
#include <vehicle.h>
#define antiClockwise Contrarotate
#define buzzerPin 33

vehicle myCar;

int SPEED = 200;
bool followState = false;
bool manual = false;

void arrowLeft()
{
    if (manual)
    {
        myCar.Move(antiClockwise, SPEED);
    }
}

void arrowRight()
{
    if (manual)
    {
        myCar.Move(Clockwise, SPEED);
    }
}

void arrowUp()
{
    if (manual)
    {
        myCar.Move(Forward, SPEED);
    }
}

void arrowDown()
{
    if (manual)
    {
        myCar.Move(Backward, SPEED);
    }
}

void but1()
{
    if (manual)
    {
        myCar.Move(Move_Left, SPEED);
    }
}

void but2()
{
}

void but3()
{
    if (manual)
    {
        myCar.Move(Move_Right, SPEED);
    }
}

void toggleFollow()
{
    if (manual == false)
    {
        followState = !followState;
    }
}

void toggleManual()
{
    if (followState == false)
    {
        manual = !manual;
    }
}