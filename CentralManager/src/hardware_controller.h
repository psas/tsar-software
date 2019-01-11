#ifndef _HARDWARE_CONTROLLER_H_
#define _HARDWARE_CONTROLLER_H_

#include <ctime>                // for delays (waits)
#include <cstring>              // memcpy
//#include <sys/resource.h>
//#include <unistd.h>
#include <stdlib.h>
#include <wiringPi.h>           // raspberry pi UART/GPIO
#include <wiringSerial.h>       // raspberry pi UART
#include <iostream>             // cout

#include "hardware_library.h"
#include "sensor_data_frame.h"
#ifdef LINK_ON
#include "link_logger.h"
#endif // LINK_ON

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
#ifdef LINK_ON
        hardware_controller(link_logger * input);
        ~hardware_controller();
#endif // LINK_ON
        
        void driver_loop();
        int kill_driver();
        int get_frame(struct sensor_data_frame & input) const;
        int light_on() const;
        int light_off() const;
    private:
#ifdef LIVE_DATA
        int i2c_setup();
#endif // LIVE_DATA
        int update_frame();
        int get_time() const;
#ifdef PRINT_DATA_FRAME
        void print_current_frame() const;
#endif // PRINT_DATA_FRAME

#ifdef LINK_ON
        link_logger * ll;
#endif // LINK_ON

#ifdef LIVE_DATA
        int adc1_fd;
#endif // LIVE_DATA

        struct timespec driver_delay;
        struct sensor_data_frame frame;
        struct sensor_fds_list fd_list;
        int frame_size;
        int epoch;
        bool driver_running;
};
#endif
