#ifndef Custom_Button_h
#define Custom_Button_h

#include "Arduino.h"

class Custom_Button{
    public:
        Custom_Button(int pin);
        void begin();
        int isPress();
    private:
        int _pin;
};

#endif