#include <IRfunctions.h>

#define leftLEDPin 2
#define rightLEDPin 12

float currentTime;
int behaviourTime;

// IDLE FUNCTIONS
void behaviour1()
{
    followState = true;
}

void behaviour2()
{
    myCar.Move(Clockwise, SPEED);
}

void behaviour3()
{
    myCar.Move(antiClockwise, SPEED);
}

void (*functionArray[7])() = {behaviour1, behaviour1, behaviour1, behaviour2, behaviour2, behaviour3, behaviour3};

void randBehaviour()
{
    if (idle)
    {
        followState = false;
        myCar.Move(Stop, 0);
        myServo.write(90);
        digitalWrite(leftLEDPin, LOW);
        digitalWrite(rightLEDPin, LOW);
        int randomIndex = random(0, 6);
        behaviourTime = random(1, 3);
        SPEED = random(120, 250);
        Serial.print("Executing function at index: ");
        Serial.println(randomIndex);
        Serial.println(SPEED);
        Serial.println(behaviourTime);
        // 5. Call the randomly selected function from the array
        functionArray[randomIndex]();

        while (currentTime < behaviourTime)
        {
            checkDistance();
            handleIRrecieve();
            Serial.println(currentTime);
            Serial.println(behaviourTime);
            UT_distance = myUltrasonic.Ranging();
            currentTime += 0.5;
            digitalWrite(leftLEDPin, HIGH);
            digitalWrite(rightLEDPin, HIGH);
            delay(500);
            digitalWrite(leftLEDPin, LOW);
            digitalWrite(rightLEDPin, LOW);
        }
        currentTime = 0;     

    }
}