#ifndef _HARDWARE_LIBRARY_H_
#define _HARDWARE_LIBRARY_H_

#ifndef LIVE_DATA
#include <cstdlib> // rand
#endif
#include <wiringPi.h> // raspberry pi I2C

namespace hardware_library {
    int example_i2c(int addr);
#ifndef LIVE_DATA
    int random_int(int addr);
    float random_float(int addr);
#endif // LIVE_DATA
}

#endif // _HARDWARE_LIBRARY_H_
