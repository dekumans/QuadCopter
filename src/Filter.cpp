#include "Arduino.h"
#include "Filter.h"

void Filter::addValue(int16_t value)
{
    buffer[position] = value;
    position++;
    if (position > (FILTER_SIZE-1)) {
        position = 0;
    }
}

int16_t Filter::getMean()
{
    uint8_t i;
    int sum = 0;

    for (i = 0; i < FILTER_SIZE; i++) {
        sum += buffer[i];
    }

    return sum / FILTER_SIZE;
}
