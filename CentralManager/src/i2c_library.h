#ifndef _I2C_LIBRARY_H_
#define _I2C_LIBRARY_H_

#include <iostream>         // cout, cerr
#include <cstdint>          // uint_t
#include <wiringPiI2C.h>    // raspberry pi I2C
#include <cstdlib>          // rand
#include <iostream>         // cerr

/*
 * This contain all hardware files for I2C testing
 */
namespace i2c_library {
    /* MPL3115A2 pressure/temperature sensor 
     * only used for testing i2c and as an example until
     * TSAR hardware is ready for testing
     */
    int MPL3115A2_setup(const int & reg);
    double MPL3115A2_pres(const int & fd);
    double MPL3115A2_temp(const int & fd);
}

#endif // _HARDWARE_LIBRARY_H_
