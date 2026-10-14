#include <Arduino.h>
#include <vehicle.h>
#define antiClockwise Contrarotate
#define buzzerPin 33
#define IRpin 4

vehicle myCar;
IRrecv myIRrecv(IRpin);
ultrasonic myUltrasonic;
Servo myServo;

int UT_distance = 0;
int SPEED = 120;
bool followState = false;
bool manual = false;
bool idle = false;
int refreshSpeed = 500;
bool recieveInstruction = true;
int leftSide;
int rightSide;
int target;
bool facingTarget = false;

unsigned long lastCommandTime;            // Record the time of the last received command
const unsigned long commandTimeout = 100; // Set the timeout period (milliseconds)
uint32_t last_decode = 0;                 // Variable to store the previously decoded raw data
uint32_t current_decode = 0;

void refreshControler()
{
    if (manual)
    {
        refreshSpeed = 500;
    }
    else
    {

            if (followState)
            {
                refreshSpeed = 5;
            }
            else
            {
                refreshSpeed = 1000;
            }
        }
    }


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

void idleToggle()
{
    idle = !idle;
}

void findLeft()
{
    while (facingTarget == false)
    {

        UT_distance = myUltrasonic.Ranging();
        myCar.Move(antiClockwise, 200);
        Serial.println(UT_distance);
        if (target - 3 <= UT_distance && target + 3 >= UT_distance)
        {
            Serial.println("FOUND");
            facingTarget = true;
        }
        delay(100);
    }
    // delay(500);
    myCar.Move(Stop, 0);
}

void findRight()
{
    while (facingTarget == false)
    {

        UT_distance = myUltrasonic.Ranging();
        myCar.Move(Clockwise, 200);
        Serial.println(UT_distance);
        if (target - 3 <= UT_distance && target + 3 >= UT_distance)
        {
            Serial.println("FOUND");
            facingTarget = true;
        }
        delay(100);
    }
    // delay(500);
    myCar.Move(Stop, 0);
}

// IR BUTTON FUNCTIONS

void findClosest()
{
    if ((followState == false) & (manual == false))
    {
        // FIND RIGHT DISTANCE
        recieveInstruction = false;
        myServo.write(0);
        delay(300);
        rightSide = myUltrasonic.Ranging();
        Serial.print(rightSide);
        Serial.println(" RIGHT");
        // FIND LEFT SIDE
        myServo.write(180);
        delay(400);
        leftSide = myUltrasonic.Ranging();
        Serial.print(leftSide);
        Serial.println(" LEFT");
        // FIND LOWEST
        target = min(leftSide, rightSide);
        myServo.write(90);
        delay(400);
        Serial.print("target: ");
        Serial.println(target);
        delay(500);
        // CHOOSE FACE SIDE
        if (target < 100)
        {
            if (target == leftSide)
            {
                findLeft();
            }
            else
            {
                findRight();
            }
        }
        // RESET
        leftSide = 0;
        rightSide = 0;
        recieveInstruction = true;
        facingTarget = false;
    }
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

void handleIRrecieve()
{
    if (recieveInstruction)
    {
        if (myIRrecv.decode())
        {
            myCar.Move(Stop, 0);
            myServo.write(90);
            lastCommandTime = millis();
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
                arrowUp();
                break;

            case 0xEA15FF00:
                arrowDown();
                break;

            case 0xBB44FF00:
                arrowLeft();
                break;
            case 0xBC43FF00:
                arrowRight();
                break;

            case 0xE916FF00:
                but1();
                break;

            case 0xF20DFF00:

                but3();
                break;

            case 0xE619FF00:
                myCar.Move(Stop, 0);
                but2();
                break;

            case 0xBD42FF00:
                findClosest();
                break;

            case 0xB54AFF00:
                toggleFollow();
                break;

            case 0xBF40FF00:
                toggleManual();
                break;

            case 0xF708FF00:
                speedDown();
                break;

            case 0xA55AFF00:
                speedUp();
                break;
            case 0xAD52FF00:
                idleToggle();
                break;
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
        delay(250);
        refreshControler();
    }
}