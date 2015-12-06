#include "Arduino.h"
#include "Motors.h"

void Motors::init()
{
    commandAll(1000);
}

void Motors::command()
{
    analogWrite(MOTORPIN0, motorCommand[0] / 8);
    analogWrite(MOTORPIN1, motorCommand[1] / 8);
    analogWrite(MOTORPIN2, motorCommand[2] / 8);
    analogWrite(MOTORPIN3, motorCommand[3] / 8);
}

void Motors::commandAll(uint16_t command)
{
    analogWrite(MOTORPIN0, command / 8);
    analogWrite(MOTORPIN1, command / 8);
    analogWrite(MOTORPIN2, command / 8);
    analogWrite(MOTORPIN3, command / 8);
}
