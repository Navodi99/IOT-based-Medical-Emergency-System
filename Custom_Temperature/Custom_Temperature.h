#ifndef Custom_Temperature_h
#define Custom_Temperature_h

#include "Arduino.h"

class Custom_Temperature{
    public:
        Custom_Temperature(int pin);
        double getTemperature();
    private:
        int _pin;
};

#endif