#ifndef _HARDWARE_LIBRARY_H_
#define _HARDWARE_LIBRARY_H_

#include <iostream>  // cout
#include <stdint>

#ifdef LIVE_DATA
#include <wiringPiI2C.h> // raspberry pi I2C
#else
#include <cstdlib> // rand
#endif

namespace hardware_library {
#ifdef LIVE_DATA
    int32_t MPL3115A2_setup(const uint32_t reg);
    float MPL3115A2_pres(const uint32_t fd);
    float MPL3115A2_temp(const uint32_t fd);
#else
    int32_t random_int();
    float random_float();
#endif // LIVE_DATA
}

#endif // _HARDWARE_LIBRARY_H_
