#ifndef _HARDWARE_CONTROLLER_H_
#define _HARDWARE_CONTROLLER_H_

#include <iostream>                     // cerr
#include <cstdint>                      // uint_t
#include <cstdlib>
#include <memory>                       // shared_ptr
#include <thread>                       // sleep_for
#include <vector>                       // sleep_for
#include <wiringPi.h>                   // raspberry pi UART/GPIO
#include <wiringSerial.h>               // raspberry pi UART

#include "../main_class.h"
#include "../hardware_data_frame.h"
#include "../link_logger.h"
#include "pi_uart.h"
#include "pi_gpio.h"
#include "i2c_library.h"
#include "i2c_data_frame.h"
#include "AC_data_frame.h"
#include "gpio_data_frame.h"

// general
#define HDW_DRIVER_DELAY 500            // mircoseconds

// raspberry pi i2c senors
#define MPL3115A2_1_ADD 0x60            // example MPL3115A2
#define MPL3115A2_2_ADD 0x63            // example non existing MPL3115A2 device

// raspberry pi gpio pins
#define LIGHT_1_GPIO 0                  // gpio17 or pin11 on pi
#define LIGHT_2_GPIO 1                  // gpio18 or pin12 on pi (non existing light for testing)
#define SEQ_EMERGENCY_LIGHT 2           // gpio21 or pin? on pi

// uart
#define BUAD_RATE 9600                  // UART buad rate
#define UART_PATH "/dev/ttyACM0"        // UART path
#define HB_TIME_MS 100                  // Time between sent heartbeats in milliseconds

// actutor controller data
#define AC_TO_CM_LEN 11                 // message length in Bytes (not package length)
#define CM_TO_AC_LEN 7                 // message length in Bytes (not package length)


// fd list for referencing by hardware controller
struct i2c_fd_list {
    int MPL3115A2_1;
    int MPL3115A2_2;
};


// GPIO list for sequencer to use to issue command to specific GPIO pins
enum eGPIO {
    eLight1,
    eLight2,
    eEmergencyLight,
    GPIO_COUNT // this must always be the last item in the enum, only used to get size
};


/*
 * Controls the hardware, the hardware interface for the sequencer.
 * It also maintains a connection to the Actuator Contoller (AC) for issuing command to.
 */
class hardware_controller : public main_class {
    public:
        // normal contructor for when not debugging
        hardware_controller(std::shared_ptr<link_logger> & input);

        // return a copy of current data frame
        void get_frame(hardware_data_frame & input);

        int light_on();
        int light_off();
        int in_emergency();
        int not_in_emergency();
        void driver_loop();
        void set_gpio_value(const int & in, const int & value);
        //TODO modify next uart message to be sent function (for sequencer to call)
    private:
        // reads sensor values, updates internal data frame
        void update_i2c_data();

        // reads gpio values, updates internal data frame
        void update_gpio_data();

        // send and reads uart message if availibe
        void update_uart_data();
        void update_AC_data(const std::vector<char> & message);

        // makes a hardware data frame
        void make_frame(hardware_data_frame & input);

        std::shared_ptr<link_logger> _ll;

        // GPIO array for sequencer to call, uses gpio enum as index
        std::vector<pi_gpio> _gpio_pins;

        // all i2c fd info
        i2c_fd_list _i2c_fds;
        int _uart_fd;

        // current data frames
        i2c_data_frame _i2c_data;
        gpio_data_frame _gpio_data;
        AC_data_frame _AC_data;

        // for uart Actuator Controller
        pi_uart _actuator_controller;
        std::vector<char> _next_uart_msg; // TODO make function to modify this
        std::chrono::system_clock::time_point _next_heartbeat_time;
};
#endif
