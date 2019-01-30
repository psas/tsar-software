#ifndef _HARDWARE_LIBRARY_H_
#define _HARDWARE_LIBRARY_H_

#include <iostream>         // cout
#include <stdint>           // uint_t
#include <wiringPiI2C.h>    // raspberry pi I2C
#include <cstdlib>          // rand

namespace hardware_library {
    int32_t MPL3115A2_setup(const uint32_t reg);
    float MPL3115A2_pres(const uint32_t fd);
    float MPL3115A2_temp(const uint32_t fd);

    int32_t random_int();
    float random_float();
}

#endif // _HARDWARE_LIBRARY_H_
