#ifndef FILTER_H
#define FILTER_H

#define FILTER_SIZE 20

class Filter {
    public:
        void addValue(int16_t value);
        int16_t getMean();

    private:
        int16_t buffer[FILTER_SIZE];
        uint8_t position;
};

#endif /* Filter.h */

