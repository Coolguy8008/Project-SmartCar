#include <Arduino.h>
#include <vehicle.h>
#define antiClockwise Contrarotate
#define buzzerPin 33

vehicle myCar;

int SPEED = 120;
bool followState = false;
bool manual = false;
bool followingLine = false;
bool idle = false;

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
    if ((followState == false) && (manual == false))
    {
        followingLine = !followingLine;
    }
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
    myCar.Move(Stop, 0);
    if (manual == false)
    {
        followState = !followState;
    }
}

void toggleManual()
{
    myCar.Move(Stop, 0);
    if (followState == false)
    {
        manual = !manual;
    }
}

void toggleLineFollow()
{
    myCar.Move(Stop, 0);
    if ((followState == false) && (manual == false))
    {
        followingLine = !followingLine;\
       
    }
}

void speedUp()
{
    SPEED += 10;
    if (SPEED > 250)
    {
        SPEED = 250;
    }
    Serial.println(SPEED);
}

void speedDown()
{
    SPEED -= 10;
    if (SPEED < 120)
    {
        SPEED = 120;
    }
    Serial.println(SPEED);
}

void idleToggle(){
    idle = !idle;
}