#include "Arduino.h"
#include "Filter.h"

void Filter::addValue(int16_t value)
{
    sum += value;
    sampleCount++;
}

float Filter::evaluate()
{
    float val = sum / sampleCount;

    sum = 0;
    sampleCount = 0;

    return val;
}
