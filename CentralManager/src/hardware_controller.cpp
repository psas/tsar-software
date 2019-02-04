#include "hardware_controller.h"

#ifdef LINK_OFF
//default constructor, only use for test/debuging
hardware_controller::
hardware_controller() : driver_running(0) {
    epoch = get_time();

    driver_delay.tv_sec = 0;
    driver_delay.tv_nsec = HDW_DRIVER_DELAY*1000;
    frame_size = sizeof(struct sensor_data_frame);

    wiringPiSetup();
    //add more as needed
    //pinMode(TEST_PIN, OUTPUT);
}
#endif // LINK_OFF

#ifndef LINK_OFF
hardware_controller::
hardware_controller(std::shared_ptr<link_logger> & input) : ll(input), driver_running(0) {
    epoch = get_time();

    driver_delay.tv_sec = 0;
    driver_delay.tv_nsec = HDW_DRIVER_DELAY*1000;

    wiringPiSetup();
    //add more as needed
    //pinMode(TEST_PIN, OUTPUT);
}


hardware_controller::
~hardware_controller() {
    ll = NULL; // central manager class will hand link logger deconstruction
}
#endif // LINK_OFF


void hardware_controller::
driver_loop() {
    i2c_setup();
    gpio_setup();

    std::cout << "Hardware Controller Running\n";

    driver_running = 1;
    while(driver_running == 1) {
        hdw_mutex.lock();

        update_frame();
        // put any nessary hardware controls befro unlock (UART etc)
    
        hdw_mutex.unlock();

#ifdef LINK_ON
        if(ll != NULL)
            ll->send(frame);
#endif // LINK_ON

        nanosleep(&driver_delay, NULL);
    }

    std::cout << "Hardware Controller Stopped\n";
    return;
}

int hardware_controller::
i2c_setup() {
    int ri =0;

    fd_list.MPL3115A2 = hardware_library::MPL3115A2_setup(MPL3115A2_ADDRESS);
    fd_list.ghost = -1; //hardware_library::ghost_setup(GHOST_SENOSR_ADDRESS);

    return ri;
}

int hardware_controller::
gpio_setup() {
    pinMode(LIGHT_GPIO, OUTPUT);
    digitalWrite(LIGHT_GPIO, LOW);
    frame.light_status = 0;
    return 1;
}


int hardware_controller::
kill_driver() { 
    driver_running = 0; 
    return 1;
}


// give a copy of data frame
int hardware_controller::
get_frame(sensor_data_frame & input) {
    hdw_mutex.lock();
    input = frame;
    hdw_mutex.unlock();
    return 1;
}


// gets monotonic time from hardware (in milliseconds)
int hardware_controller::
get_time() const {
    struct timespec current_time;
    clock_gettime(CLOCK_MONOTONIC, &current_time);
    return (int)(current_time.tv_sec*1000 + current_time.tv_nsec/1000000);
}


// build new sensor frame 
int hardware_controller::
update_frame() {
    frame.time = get_time() - epoch;
    frame.pres_1 = hardware_library::MPL3115A2_pres(fd_list.MPL3115A2);
    frame.temp_1 = hardware_library::MPL3115A2_temp(fd_list.MPL3115A2);
    frame.random_int = hardware_library::random_int();
    frame.random_float = hardware_library::random_float();

    return 1;
}


// make gpio pin voltage high
int hardware_controller::
light_on() {
    hdw_mutex.lock();
    
    digitalWrite(LIGHT_GPIO, HIGH);
    frame.light_status = 1;
    
    hdw_mutex.unlock();
    return 1;
}


// make gpio pin voltage low
int hardware_controller::
light_off() {
    hdw_mutex.lock();
    
    digitalWrite(LIGHT_GPIO, LOW);
    frame.light_status = 0;
        
    hdw_mutex.unlock();
    return 1;
}
