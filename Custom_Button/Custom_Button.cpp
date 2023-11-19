#include "Arduino.h"
#include "Custom_button.h"

Custom_Button::Custom_Button(int pin){
    _pin=pin;
}

void Custom_Button::begin(){
    pinMode(_pin,INPUT);
}

int Custom_Button::isPress(){
    return digitalRead(_pin);
}