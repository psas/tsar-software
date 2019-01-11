#ifndef _HARDWARE_LIBRARY_H_
#define _HARDWARE_LIBRARY_H_

#include <iostream>  // cout

#ifdef LIVE_DATA
#include <wiringPiI2C.h> // raspberry pi I2C
#else
#include <cstdlib> // rand
#endif

namespace hardware_library {
#ifdef LIVE_DATA
    int MPL3115A2_setup(int reg);
    float MPL3115A2_pres(int fd);
    float MPL3115A2_temp(int fd);
#else
    int random_int();
    float random_float();
#endif // LIVE_DATA
}

#endif // _HARDWARE_LIBRARY_H_
