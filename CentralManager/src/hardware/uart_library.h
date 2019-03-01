#ifndef _UART_LIBRARY_H_
#define _UART_LIBRARY_H_

#include <iostream>                 // cout, cerr
#include <cstdint>                  // uint_t
#include <wiringSerial.h>           // raspberry pi uart
#include <cstring>                  // memcpy
#include <string>                   // std string lib

#include "AC_data_frame.h"

// UART data lengths
#define AC_TO_CM_LEN 16             // message length in Bytes
#define CM_TO_AC_LEN 7              // message length in Bytes


/*
 * This contain all uart functions
 */
namespace uart_library {
    /* returns 1 and overrides data if a uart message is availible
     * return 0 if no message, -1 for error
     */
    int read(AC_data_frame & data, const int & fd);

    /* send default no change command to device
     * TODO add a input command option
     */
    void send_default(const int & fd);
    //void send(const CM_UART_message & mes, const int & fd); // TODO implement this
}

#endif // _UART_LIBRARY_H_
