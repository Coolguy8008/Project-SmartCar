#include <IRfunctions.h>

#define leftLEDPin 2
#define rightLEDPin 12
#define crashPin 34

int crashState;
float currentTime;
int behaviourTime;

// IDLE FUNCTIONS
void forward()
{
    lcd.print(__func__);

    myCar.Move(Forward, SPEED);
}

void right()
{
    lcd.print(__func__);

    myCar.Move(Clockwise, SPEED);
}

void left()
{
    lcd.print(__func__);

    myCar.Move(antiClockwise, SPEED);
}

void (*functionArray[6])() = {forward, forward, right, right, left, left};

void randBehaviour()
{
    if (idle)
    {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Chosen Behaviour: ");
        lcd.setCursor(0, 1);
        myCar.Move(Stop, 0);
        currentTime = 0;
        digitalWrite(leftLEDPin, LOW);
        digitalWrite(rightLEDPin, LOW);
        int randomIndex = random(0, 5);
        behaviourTime = random(1, 3);
        SPEED = random(120, 250);

        // 5. Call the randomly selected function from the array
        functionArray[randomIndex]();

        while (currentTime < behaviourTime)
        {
            crashState = digitalRead(crashPin);
            if (crashState == LOW)
            {
                myCar.Move(Backward, 250);
                delay(1000);
                myCar.Move(Stop, 0);
                randBehaviour();
            }
            else
            {
                handleIRrecieve();
                UT_distance = myUltrasonic.Ranging();
                currentTime += 0.1;
                delay(100);
            }
        }
        currentTime = 0;
    }
}