#include "hardware_controller.h"

#ifdef LINK_OFF
//default constructor, only use for test/debuging
hardware_controller::
hardware_controller() : driver_running(0) {
    epoch = get_time();
    wiringPiSetup();
    //add more as needed
    //pinMode(TEST_PIN, OUTPUT);
}
#endif // LINK_OFF


#ifndef LINK_OFF
hardware_controller::
hardware_controller(std::shared_ptr<link_logger> & input) : ll(input), driver_running(0) {
    epoch = get_time();
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

        std::this_thread::sleep_for(std::chrono::microseconds(HDW_DRIVER_DELAY));
    }

    return;
}


int hardware_controller::
i2c_setup() {
    int ri =0;

    fd_list.MPL3115A2_1 = hardware_library::MPL3115A2_setup(MPL3115A2_1_ADD);
    fd_list.MPL3115A2_2 = hardware_library::MPL3115A2_setup(MPL3115A2_2_ADD);

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
    auto time = std::chrono::steady_clock::now();
    auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(time);
    auto value = now_ms.time_since_epoch();
    long duration = value.count();
    return 1;
}



#ifndef LIVE_DATA_OFF
// build new sensor frame from live sensors 
int hardware_controller::
update_frame() {
    frame.time = get_time() - epoch;
    frame.pres_1 = hardware_library::MPL3115A2_pres(fd_list.MPL3115A2_1);
    frame.temp_1 = hardware_library::MPL3115A2_temp(fd_list.MPL3115A2_1);
    frame.pres_2 = hardware_library::MPL3115A2_pres(fd_list.MPL3115A2_2);
    frame.temp_2 = hardware_library::MPL3115A2_temp(fd_list.MPL3115A2_2);

    return 1;
}
#else
// build new sensor frame from a file, only for testing/debugging
// this is used to automate a controlled test of hardware or system
int hardware_controller::
update_frame() {
    // read for file for automated test
    // TODO: replace with file in later
    frame.time = get_time() - epoch;
    frame.pres_1 = 1.0;
    frame.temp_1 = 1.0;
    
    return 1;
}
#endif // LIVE_DATA_OFF


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
