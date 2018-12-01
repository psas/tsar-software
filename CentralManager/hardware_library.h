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
    int mcp3424(int addr, int reg);
#else
    int random_int();
    float random_float();
#endif // LIVE_DATA
}

#endif // _HARDWARE_LIBRARY_H_
