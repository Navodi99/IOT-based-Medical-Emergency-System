#ifndef Custom_HeartRate_h
#define Custom_HeratRate_h

#include "Arduino.h"
#define DHTPIN 2
#define USE_ARDUINO_INTERRUPTS true


class Custom_HeartRate{
    public:
        Custom_HeartRate(int pulseWire,int threshold);
        int getHeartRate();
        void setup();
    private:
        int _pulsWire;
        int _threshold;
};
#endif