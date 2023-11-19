#include "Arduino.h"
#include "Custom_Temperature.h"

Custom_Temperature::Custom_Temperature(int pin){
    _pin=pin;
}

double Custom_Temperature::getTemperature(){
    int temp_adc_val;
    float temp_val;
    temp_adc_val = analogRead(_pin);
    temp_val = (temp_adc_val * 4.88);      
    return (temp_val/10); 
}