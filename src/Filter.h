#ifndef FILTER_H
#define FILTER_H

class Filter {
    public:
        void addValue(int16_t value);
        float evaluate();

    private:
        int32_t sum;
        uint16_t sampleCount;
};

#endif /* Filter.h */

