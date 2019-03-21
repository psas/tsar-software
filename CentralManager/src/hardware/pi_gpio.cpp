#include "pi_gpio.h"

// default constructor, set ouput value (HIGH/LOW)
pi_gpio::
pi_gpio(const int & pin, const int & value) : _pin(pin) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, value);
    _status = digitalRead(pin);
}


void pi_gpio::
set_value(const int & value) {
    digitalWrite(_pin, value);
    _status = digitalRead(_pin);
}


int pi_gpio::
get_status(){
    _status = digitalRead(_pin);
    return _status;
}
