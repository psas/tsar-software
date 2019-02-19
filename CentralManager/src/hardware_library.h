#ifndef _HARDWARE_LIBRARY_H_
#define _HARDWARE_LIBRARY_H_

#include <iostream>                 // cout, cerr
#include <cstdint>                  // uint_t
#include <wiringPiI2C.h>            // raspberry pi I2C
#include <iostream>                 // cerr
#include <cstring>                  // memcpy
#include <array>                    // array

#include "sensor_data_frame.h"      // AC_data_frame

// UART params
#define AC_TO_CM_LEN 16             // message length in Bytes
#define CM_TO_AC_LEN 7              // message length in Bytes

#define AC_CMD_DO_NOTHING 0
#define AC_CMD_PANIC 1

#define AC_FM_NO_FAILURE 0
#define AC_FM_OTHER_FAILURE 255


/*
 * This contain all hardware files for I2C and UART
 */
namespace hardware_library {
    /* MPL3115A2 pressure/temperature sensor 
     * only used for testing i2c and as an example until
     * TSAR hardware is ready for testing
     */
    int MPL3115A2_setup(const int & reg);


    // returns pressure (in kPa) from mpl3115a2
    double MPL3115A2_pres(const int & fd);

    // returns temperature (in C) from mpl3115a2
    double MPL3115A2_temp(const int & fd);


    /* returns 1 and overrides data if a uart message is availible
     * return 0 if no message, -1 for error
     */
    int uart_update(AC_data_frame & data, const int & fd);

    /* send defautl no change command to device
     * TODO add a input command option
     */
    int uart_send_command(int fd);
}

#endif // _HARDWARE_LIBRARY_H_
