#include "Filter.h"

void Filter::init(float input, float output)
{
    filterData.inputTm1 = input;
    filterData.inputTm2 = input;
    filterData.inputTm3 = input;
    filterData.inputTm4 = input;
    
    filterData.outputTm1 = output;
    filterData.outputTm2 = output;
    filterData.outputTm3 = output;
    filterData.outputTm4 = output;
}

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

    lastValue = val;

    return val;
}

float Filter::computeFilter(float input)
{
    #define _b0  0.001893594048567
    #define _b1 -0.002220262954039
    #define _b2  0.003389066536478
    #define _b3 -0.002220262954039
    #define _b4  0.001893594048567
    
    #define _a1 -3.362256889209355
    #define _a2  4.282608240117919
    #define _a3 -2.444765517272841
    #define _a4  0.527149895089809

    float output;
    
    output =  _b0 * input                + 
              _b1 * filterData.inputTm1  + 
              _b2 * filterData.inputTm2  +
              _b3 * filterData.inputTm3  +
              _b4 * filterData.inputTm4  -
              _a1 * filterData.outputTm1 -
              _a2 * filterData.outputTm2 -
              _a3 * filterData.outputTm3 -
              _a4 * filterData.outputTm4;
    
    filterData.inputTm4 = filterData.inputTm3;
    filterData.inputTm3 = filterData.inputTm2;
    filterData.inputTm2 = filterData.inputTm1;
    filterData.inputTm1 = input;
    
    filterData.outputTm4 = filterData.outputTm3;
    filterData.outputTm3 = filterData.outputTm2;
    filterData.outputTm2 = filterData.outputTm1;
    filterData.outputTm1 = output;
    
    return output;
}
