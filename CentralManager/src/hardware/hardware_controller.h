#ifndef _HARDWARE_CONTROLLER_H_
#define _HARDWARE_CONTROLLER_H_

#include <iostream>                     // cerr
#include <cstdint>                      // uint_t
#include <cstdlib>
#include <memory>                       // shared_ptr
#include <thread>                       // sleep_for
#include <wiringPi.h>                   // raspberry pi UART/GPIO
#include <wiringSerial.h>               // raspberry pi UART

#include "../main_class.h"
#include "../hardware_data_frame.h"
#include "../link_logger.h"
#include "i2c_library.h"
#include "uart_library.h"

// general
#define HDW_DRIVER_DELAY 500            // mircoseconds

// raspberry pi i2c senors
#define MPL3115A2_1_ADD 0x60            // example MPL3115A2
#define MPL3115A2_2_ADD 0x63            // example non existing MPL3115A2

// raspberry pi gpio pins
#define LIGHT_GPIO 0                    // gpio 17 or pin 11 on pi

// uart
#define BUAD_RATE 9600                  // UART buad rate
#define UART_PATH "/dev/ttyAMA0"        // UART path
#define HB_TIME_MS 10                   // Time between sent heartbeats in milliseconds


// fd list for referencing by hardware controller
struct i2c_fd_list {
    int MPL3115A2_1;
    int MPL3115A2_2;
};


/*
 * Controls the hardware, the hardware interface for the sequencer.
 * It also maintains a connection to the Actuator Contoller (AC) for issuing command to.
 */
class hardware_controller : public main_class {
    public:
        // normal contructor for when not debugging
        hardware_controller(std::shared_ptr<link_logger> & input);
        ~hardware_controller();

        // return a copy of current data frame
        void get_frame(hardware_data_frame & input);

        int light_on();
        int light_off();
        void driver_loop();
    private:
        // reads sensor values, updates internal data frame
        void update_i2c_data();

        // makes a hardware data frame
        void make_frame(hardware_data_frame & input);

        std::shared_ptr<link_logger> _ll;

        // all fd info
        i2c_fd_list _i2c_fds;
        int _uart_fd;

        // current data frames
        i2c_data_frame _i2c_data;
        bool _light_status;
        bool _AC_connected;
        AC_data_frame _AC_data;

        std::chrono::system_clock::time_point _next_heartbeat_time;
};
#endif
