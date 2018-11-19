#ifndef _HARDWARE_CONTROLLER_H_
#define _HARDWARE_CONTROLLER_H_

#include <ctime>
#include <cstring>
#include <sys/resource.h>
#include <unistd.h>
#include <stdlib.h>
#include <wiringSerial.h> // raspberry pi UART
#include <iostream> 

#include "hardware_library.h"
#include "sensor_data_frame.h"
#ifdef LINK_ON
#include "link_logger.h"
#endif // LINK_ON

#define HDW_DRIVER_DELAY 500000 // Mircoseconds

// raspberry pi gpio pins
#define TEST_PIN 1 // example

// raspberry pi i2c senors
#define TEST_ADDR 0x00 // for testing, DO NOT REMOVE
#define THERMO0 0x68 // example


class hardware_controller {
    public:
        hardware_controller();
#ifdef LINK_ON
        hardware_controller(link_logger * input);
        ~hardware_controller();
#endif // LINK_ON
        
        void driver_loop();
        int kill_driver();
        int get_frame(struct sensor_data_frame & input) const;
    private:
        int update_frame();
        int get_time() const;
#ifdef PRINT_DATA_FRAME
        void print_current_frame() const;
#endif // PRINT_DATA_FRAME

#ifdef LINK_ON
        link_logger * ll;
#endif // LINK_ON

        struct timespec driver_delay;
        struct sensor_data_frame frame;
        int frame_size;
        int epoch;
        bool driver_running;
};
#endif
