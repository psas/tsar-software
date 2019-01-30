#ifndef _HARDWARE_CONTROLLER_H_
#define _HARDWARE_CONTROLLER_H_

#include <ctime>                // for delays (waits)
#include <cstring>              // memcpy
#include <cstdint>
//#include <sys/resource.h>
//#include <unistd.h>
#include <mutex>                // muxtex
#include <stdlib.h>
#include <wiringPi.h>           // raspberry pi UART/GPIO
#include <wiringSerial.h>       // raspberry pi UART
#include <iostream>             // cout, cerr

#include "hardware_library.h"
#include "sensor_data_frame.h"
#ifndef LINK_OFF
#include "link_logger.h"
#endif // LINK_OFF

#define HDW_DRIVER_DELAY 5000   // Mircoseconds
#define BUAD_RATE 115200        // UART buad rate
#define PATH "/dev/ttyAMA0"     // UART path

// raspberry pi gpio pins
#define LIGHT_GPIO 0            //gpio 17 or pin 11 on pi

// raspberry pi i2c senors
#define MPL3115A2_ADDRESS 0x60              // example MPL3115A2
#define GHOST_SENSOR_ADDRESS 0x00           // sensor that does not exist for testing only

struct sensor_fds_list {
    int MPL3115A2;
    int ghost;
};


class hardware_controller {
    public:
        hardware_controller();
        hardware_library(const hardware_controller &) = delete;
#ifndef LINK_OFF
        hardware_controller(link_logger * input);
        ~hardware_controller();
#endif // LINK_OFF
        
        void driver_loop();
        int kill_driver();
        int get_frame(struct sensor_data_frame & input) const;
        int light_on() const;
        int light_off() const;
    private:
        int i2c_setup();
        int gpio_setup();
        int update_frame();
        int get_time() const;
#ifdef PRINT_DATA_FRAME
        void print_current_frame() const;
#endif // PRINT_DATA_FRAME

#ifndef LINK_OFF
        link_logger * ll;
#endif // LINK_OFF
        timespec driver_delay;
        sensor_data_frame frame;
        sensor_fds_list fd_list;
        int frame_size;
        int epoch;
        bool driver_running;
        std::mutex hdw_mutex;
};
#endif
