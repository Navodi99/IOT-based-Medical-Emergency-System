#include "Arduino.h"
#include "Custom_HeartRate.h"
#include <PulseSensorPlayground.h>

PulseSensorPlayground pulseSensor;

Custom_HeartRate::Custom_HeartRate(int pulseWire,int threshold){
    _pulsWire=pulseWire;
    _threshold=threshold;
}

void Custom_HeartRate::setup(){
    pulseSensor.analogInput(_pulsWire);
    pulseSensor.setThreshold(_threshold);
    pulseSensor.begin();
}

int Custom_HeartRate::getHeartRate(){
    int myBPM = pulseSensor.getBeatsPerMinute();
    if(pulseSensor.sawStartOfBeat()){
        return myBPM;
    }

    return 70;
}