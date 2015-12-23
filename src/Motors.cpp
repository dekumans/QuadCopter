#include "Motors.h"

void Motors::init()
{
    commandAll(1000);
}

void Motors::command()
{
    analogWrite(MOTORPIN0, motorCommand[MOTOR_FR] / 8);
    analogWrite(MOTORPIN1, motorCommand[MOTOR_FL] / 8);
    analogWrite(MOTORPIN2, motorCommand[MOTOR_RR] / 8);
    analogWrite(MOTORPIN3, motorCommand[MOTOR_RL] / 8);
}

void Motors::commandAll(uint16_t command)
{
    motorCommand[MOTOR_FR] = command;
    motorCommand[MOTOR_FL] = command;
    motorCommand[MOTOR_RR] = command;
    motorCommand[MOTOR_RL] = command;

    analogWrite(MOTORPIN0, command / 8);
    analogWrite(MOTORPIN1, command / 8);
    analogWrite(MOTORPIN2, command / 8);
    analogWrite(MOTORPIN3, command / 8);
}

void Motors::printMotorCommands()
{
    Serial1.print("Motors");
    Serial1.print(";");
    Serial1.print(motorCommand[MOTOR_FR], DEC);
    Serial1.print(";");
    Serial1.print(motorCommand[MOTOR_FL], DEC);
    Serial1.print(";");
    Serial1.print(motorCommand[MOTOR_RR], DEC);
    Serial1.print(";");
    Serial1.println(motorCommand[MOTOR_RL], DEC);
}
