#include "wiringPi.h"

struct pi_gpio {
    pi_gpio(const int & pin, const int & value);
    void set_value(const int & value);
    int get_status();
    private:
        int _pin;
        int _status;

};
