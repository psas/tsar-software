#ifndef _I2C_LIBRARY_H_
#define _I2C_LIBRARY_H_

#include <iostream>                 // cout, cerr
#include <cstdint>                  // uint_t
#include <wiringPiI2C.h>            // raspberry pi I2C


/*
 * This contain all I2C functions
 */
namespace i2c_library {
    /* MPL3115A2 pressure/temperature sensor 
     * only used for testing i2c and as an example until
     * TSAR hardware is ready for testing
     */
    int MPL3115A2_setup(const int & reg);


    // returns pressure (in kPa) from mpl3115a2
    double MPL3115A2_pres(const int & fd);

    // returns temperature (in C) from mpl3115a2
    double MPL3115A2_temp(const int & fd);
}

#endif // _I2C_LIBRARY_H_
