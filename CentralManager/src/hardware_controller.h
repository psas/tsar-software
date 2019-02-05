#ifndef _HARDWARE_CONTROLLER_H_
#define _HARDWARE_CONTROLLER_H_

#include <cstdint>                      // uint_t
#include <mutex>                        // muxtex
#include <cstdlib>
#include <iostream>                     // cout, cerr
#include <memory>                       // shared_ptr
#include <chrono>                       // mircoseconds
#include <thread>                       // sleep_for
#include <wiringPi.h>                   // raspberry pi UART/GPIO
#include <wiringSerial.h>               // raspberry pi UART

#include "hardware_library.h"
#include "sensor_data_frame.h"
#ifndef LINK_OFF
#include "link_logger.h"
#endif // LINK_OFF

#define HDW_DRIVER_DELAY 500            // mircoseconds
#define BUAD_RATE 115200                // UART buad rate
#define UART_PATH "/dev/ttyAMA0"        // UART path
#define TEST_SENSOR_PATH "tests/test_sensor_values.txt" // path to test sensor value

// raspberry pi gpio pins
#define LIGHT_GPIO 0            //gpio 17 or pin 11 on pi

// raspberry pi i2c senors
#define MPL3115A2_1_ADD 0x60              // example MPL3115A2
#define MPL3115A2_2_ADD 0x63              // example not existing MPL3115A2


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
#ifdef LINK_OFF
        // only use if test/debuging only the hardware controller
        hardware_controller();
#else 
        // normal contructor for when not debugging
        hardware_controller(std::shared_ptr<link_logger> & input);
        ~hardware_controller();
#endif // LINK_OFF

        void driver_loop();
        int kill_driver();

        // return a copy of current data frame
        int get_frame(sensor_data_frame & input);

        int light_on();
        int light_off();
    private:
        int i2c_setup();
        int gpio_setup();
        int update_frame();
        int get_time() const;

#ifndef LINK_OFF
        // allows fo debugin without link logger
        std::shared_ptr<link_logger> ll;
#endif // LINK_OFF

        sensor_data_frame frame;
        sensor_fds_list fd_list;
        int frame_size;
        int epoch;
        bool driver_running;
        std::mutex hdw_mutex;
};
#endif
