#ifndef _HARDWARE_LIBRARY_H_
#define _HARDWARE_LIBRARY_H_

#include <iostream>         // cout
#include <cstdint>           // uint_t
#include <wiringPiI2C.h>    // raspberry pi I2C
#include <cstdlib>          // rand

/*
 * This contain all hardware files for I2C testing
 */

namespace hardware_library {
    /* MPL3115A2 pressure/temperature sensor 
     * only used for testing i2c and as an example until
     * TSAR hardware is ready for testing
     */
    int32_t MPL3115A2_setup(const uint32_t reg);
    float MPL3115A2_pres(const uint32_t fd);
    float MPL3115A2_temp(const uint32_t fd);
}

#endif // _HARDWARE_LIBRARY_H_
