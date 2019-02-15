#include "hardware_controller.h"


// constructor, need pointer to link
hardware_controller::
hardware_controller(std::shared_ptr<link_logger> & input) : ll(input), driver_running(false) {
    wiringPiSetup();

    // setup uart
    uart_fd = serialOpen(UART_PATH, BUAD_RATE);

    // setup i2c sensors
    fd_list.MPL3115A2_1 = i2c_library::MPL3115A2_setup(MPL3115A2_1_ADD);
    fd_list.MPL3115A2_2 = i2c_library::MPL3115A2_setup(MPL3115A2_2_ADD);

    // setup gpio pins
    pinMode(LIGHT_GPIO, OUTPUT);
    digitalWrite(LIGHT_GPIO, LOW); // default value
    frame.light_status = false;
}

hardware_controller::
~hardware_controller() {
    serialClose(uart_fd);
}



void hardware_controller::
driver_loop() {
    driver_running = true;
    while(driver_running) {
        hdw_mutex.lock();

        update_frame();

        // TODO UART heartbeat here

        if(ll != NULL)
            ll->send(frame);

        hdw_mutex.unlock();
        std::this_thread::sleep_for(std::chrono::microseconds(HDW_DRIVER_DELAY));
    }
    return;
}


void hardware_controller::
stop_driver() { 
    hdw_mutex.lock();
    driver_running = false;
    hdw_mutex.unlock();
}


// give a copy of data frame
int hardware_controller::
get_frame(sensor_data_frame & input) {
    hdw_mutex.lock();
    input = frame;
    hdw_mutex.unlock();
    return 1;
}


// gets monotonic time from hardware (in microseconds) as a string
void hardware_controller::
get_time_us(std::string & time) const {
    auto now = std::chrono::steady_clock::now();
    auto now_us = std::chrono::time_point_cast<std::chrono::microseconds>(now);
    auto epoch = now_us.time_since_epoch();
    auto value = std::chrono::duration_cast<std::chrono::microseconds>(epoch);
    long long duration = value.count();
    time = std::to_string(duration);
}


#ifndef SENSOR_DATA_OFF
// build new sensor frame from live sensors 
int hardware_controller::
update_frame() {
    get_time_us(frame.time);

    // TODO deal with sensor deconnecting
    frame.pres_1 = i2c_library::MPL3115A2_pres(fd_list.MPL3115A2_1);
    frame.temp_1 = i2c_library::MPL3115A2_temp(fd_list.MPL3115A2_1);
    frame.pres_2 = i2c_library::MPL3115A2_pres(fd_list.MPL3115A2_2);
    frame.temp_2 = i2c_library::MPL3115A2_temp(fd_list.MPL3115A2_2);

    return 1;
}
#else
// build new sensor frame from a file, only for testing/debugging
// this is used to automate a controlled test of hardware or system
int hardware_controller::
update_frame() {
    // read for file for automated test
    float f_list[4];
    int d, i = 0;
    std::string s;
    
    ifstream file(TEST_SENSOR_PATH);
    if(file.is_open()) {
        while(getline(file, s) && i < 4) {
            d = s.find("=")+1;
            f_list[i] = std::stof(s.substr(d));
            ++i;
        }
        file.close();
    }

    get_time_us(frame.time);
    frame.temp_1 = f_list[0];
    frame.pres_1 = f_list[1];
    frame.temp_2 = f_list[2];
    frame.pres_2 = f_list[3];

    return 1;
}
#endif // SENSOR_DATA_OFF


// make gpio pin voltage high
int hardware_controller::
light_on() {
    hdw_mutex.lock();

    digitalWrite(LIGHT_GPIO, HIGH);
    frame.light_status = true;

    hdw_mutex.unlock();
    return 1;
}


// make gpio pin voltage low
int hardware_controller::
light_off() {
    hdw_mutex.lock();

    digitalWrite(LIGHT_GPIO, LOW);
    frame.light_status = false;

    hdw_mutex.unlock();
    return 1;
}


int hardware_controller::
uart_send(std::string & message) {
    serialPuts(uart_fd, &message[0]);
    return 1;
}
