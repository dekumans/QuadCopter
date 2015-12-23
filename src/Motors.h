#ifndef MOTORS_H
#define MOTORS_H

#include "Arduino.h"

#define MOTORPIN0   7
#define MOTORPIN1   8
#define MOTORPIN2   6
#define MOTORPIN3   11

#define MOTOR_FR    0
#define MOTOR_FL    1
#define MOTOR_RR    2
#define MOTOR_RL    3

class Motors
{
    public:
        void init();
        void command();
        void commandAll(uint16_t command);
        void printMotorCommands();

        uint16_t motorCommand[4] = { 0, 0, 0, 0 };
    
    private:

};

#endif /* Motors.h */
