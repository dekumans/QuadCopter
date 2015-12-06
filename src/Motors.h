#ifndef MOTORS_H
#define MOTORS_H

#define MOTORPIN0   6
#define MOTORPIN1   11
#define MOTORPIN2   7
#define MOTORPIN3   8

class Motors
{
    public:
        void init();
        void command();
        void commandAll(uint16_t command);

        uint16_t motorCommand[4] = { 0, 0, 0, 0 };
    
    private:

};

#endif /* Motors.h */
