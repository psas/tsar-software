#ifndef _HARDWARE_CONTROLLER_H_
#define _HARDWARE_CONTROLLER_H_

#include <iostream>                     // cerr
#include <cstdint>                      // uint_t
#include <mutex>                        // muxtex
#include <cstdlib>
#include <memory>                       // shared_ptr
#include <chrono>                       // mircoseconds
#include <thread>                       // sleep_for
#include <wiringPi.h>                   // raspberry pi UART/GPIO
#include <wiringSerial.h>               // raspberry pi UART
#include <fstream>                      // read file, only for test update()
#include <string>                       // string
#include <atomic>                       // atomic
#include <string>                       // string (uart)

#include "i2c_library.h"
#include "sensor_data_frame.h"
#include "link_logger.h"

#define HDW_DRIVER_DELAY 500            // mircoseconds
#define TEST_SENSOR_PATH "tests/test_sensor_values.txt" // path to test sensor value

// uart
#define BUAD_RATE 115200                // UART buad rate
#define UART_PATH "/dev/ttyAMA0"        // UART path

// raspberry pi gpio pins
#define LIGHT_GPIO 0                    // gpio 17 or pin 11 on pi

// raspberry pi i2c senors
#define MPL3115A2_1_ADD 0x60            // example MPL3115A2
#define MPL3115A2_2_ADD 0x63            // example non existing MPL3115A2


// fd list for referencing by hardware controller
struct sensor_fds_list {
    int MPL3115A2_1;
    int MPL3115A2_2;
};


/*
 * Controls the hardware, the hardware interface for the sequencer.
 * When testing/debuging this class can be compiled with and without link_logger.
 */
class hardware_controller {
    public:
        // normal contructor for when not debugging
        hardware_controller(std::shared_ptr<link_logger> & input);
        ~hardware_controller();

        void driver_loop();
        void stop_driver();

        // return a copy of current data frame
        int get_frame(sensor_data_frame & input);

        int light_on();
        int light_off();
    private:
        // reads sensor values, updates internal data frame
        int update_frame();

        // gets current time in microseconds as a string
        void get_time_us(std::string & time) const;

        // UART send string data 
        int uart_send(std::string & message);

        std::shared_ptr<link_logger> ll;
        sensor_data_frame frame;
        sensor_fds_list fd_list;
        int uart_fd;
        std::atomic<bool> driver_running;
        std::mutex hdw_mutex;
};
#endif
