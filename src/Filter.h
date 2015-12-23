#ifndef FILTER_H
#define FILTER_H

#include "Arduino.h"

typedef struct fourthOrderData_t {
    float inputTm1, inputTm2, inputTm3, inputTm4;
    float outputTm1, outputTm2, outputTm3, outputTm4;
} fourthOrderData;

class Filter {
    public:
        void init(float input, float output);
        void addValue(int16_t value);
        float evaluate();
        float computeFilter(float input);

        float lastValue;

    private:
        int32_t sum;
        uint16_t sampleCount;
        fourthOrderData filterData;
};

#endif /* Filter.h */

