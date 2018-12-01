#ifndef _HARDWARE_CONTROLLER_H_
#define _HARDWARE_CONTROLLER_H_

#include <ctime>
#include <cstring>
#include <sys/resource.h>
#include <unistd.h>
#include <stdlib.h>
#include <wiringSerial.h> // raspberry pi UART
#include <wiringPi.h>
#include <iostream> 

#include "hardware_library.h"
#include "sensor_data_frame.h"
#ifdef LINK_ON
#include "link_logger.h"
#endif // LINK_ON

#define HDW_DRIVER_DELAY 5000 // Mircoseconds

// raspberry pi gpio pins
#define LIGHT_GPIO 0 //gpio 17 or pin 11 on pi

// raspberry pi i2c senors
#define ADC1 0x68 // example


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
        int frame_size;
        int epoch;
        bool driver_running;
};
#endif
