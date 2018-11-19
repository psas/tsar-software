#include "hardware_controller.h"


//default constructor
hardware_controller::
hardware_controller() : driver_running(0) {
#ifdef LINK_ON
    ll = NULL;
#endif // LINK_ON
    //wiringPiSetup();

    epoch = get_time();

    driver_delay.tv_sec = 0;
    driver_delay.tv_nsec = HDW_DRIVER_DELAY*1000;
    frame_size = sizeof(struct sensor_data_frame);

    //add more as needed
    //pinMode(TEST_PIN, OUTPUT);
}

#ifdef LINK_ON
hardware_controller::
hardware_controller(link_logger * input) : ll(input), driver_running(0) {
    //wiringPiSetup();

    epoch = get_time();

    driver_delay.tv_sec = 0;
    driver_delay.tv_nsec = HDW_DRIVER_DELAY*1000;
    frame_size = sizeof(struct sensor_data_frame);

    //add more as needed
    //pinMode(TEST_PIN, OUTPUT);
}
#endif // LINK_ON

#ifdef LINK_ON
hardware_controller::
~hardware_controller() {
    ll = NULL;
}
#endif // HARDHWARE TESTS



void hardware_controller::
driver_loop() {
    driver_running = 1;
    while(driver_running == 1) {
        update_frame();
#ifdef PRINT_DATA_FRAME
        print_current_frame();
#endif
#ifdef LINK_ON
        if(ll != NULL)
            ll->send(frame);
#endif // LINK_ON
        nanosleep(&driver_delay, NULL);
    }
    return;
}


#ifdef PRINT_DATA_FRAME
void hardware_controller::
print_current_frame() const {
#ifdef LIVE_DATA
    std::cout << "time: " << frame.time << "no sensor data\n";
#else
    std::cout << "time: " << frame.time << ", int0: " << frame.test_int_0 << ", int1: " << frame.test_int_1 
         << ", float0: " << frame.test_float_0 << ", float1 " << frame.test_float_1 << std::endl;
#endif // LIVE_DATA
}
#endif // PRINT_DATA_FRAME


int hardware_controller::
kill_driver() { 
    driver_running = 0; 
    return 1;
}


int hardware_controller::
get_frame(struct sensor_data_frame & input) const {
    memcpy(&input, &frame, frame_size);
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

#ifdef LIVE_DATA
    printf("no sensors yet\n");
#else
    frame.test_int_0 = hardware_library::random_int(TEST_ADDR);
    frame.test_int_1 = hardware_library::random_int(TEST_ADDR);
    frame.test_float_0 = hardware_library::random_float(TEST_ADDR);
    frame.test_float_1 = hardware_library::random_float(TEST_ADDR);
#endif // LIVE_DATA

    return 1;
}
