#include "hardware_controller.h"


// constructor, need pointer to link
hardware_controller::
hardware_controller(std::shared_ptr<link_logger> & input) : ll(input), driver_running(false) noexcept {
    wiringPiSetup()

    // setup uart
    uart_fd = serialOpen(UART_PATH, BUAD_RATE);

    // setup i2c sensors
    fd_list.MPL3115A2_1 = i2c_library::MPL3115A2_setup(MPL3115A2_1_ADD);
    fd_list.MPL3115A2_2 = i2c_library::MPL3115A2_setup(MPL3115A2_2_ADD);

    // setup gpio pins
    pinMode(LIGHT_GPIO, OUTPUT);
    digitalWrite(LIGHT_GPIO, LOW);  // init value (LOW/HIGH)
    frame.light_status = false;     // init state
}

hardware_controller::
~hardware_controller() {
    serialClose(uart_fd);
}


void hardware_controller::
driver_loop() {
    std::chrono::system_clock::time_point current_time;
    std::array<uint8_t, AC_TO_CM_LEN>  AC_message;
    std::array<uint8_t, CM_TO_AC_LEN>  CM_message;

    driver_running = true;
    while(driver_running) {
        hdw_mutex.lock();

        update_frame();

        read_uart_message(AC_message); // TODO check for heartbeat, deal with errors

        current_time = std::chrono::system_clock::now();
        if(current_time >= next_heartbeat_time) {
            make_uart_message(CM_message);
            serialPuts(uart_fd, &message[0]); // send message
            next_heartbeat_time = current_time + std::chrono::milliseconds(10);
        }

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


// TODO change default vs sequencer command
int hardware_controller::
make_uart_message(std::array<uint8_t, CM_TO_AC_LEN> & message) {
    message[0] = '\n';
    message[1] = AC_FM_NO_FAILURE;      // Next Failure Mode
    message[2] = AC_CMD_DO_NOTHING;     // Command
    message[3] = 1;                     // sensor 1
    message[4] = 1;                     // sensor 2
    message[5] = 1;                     // sensor 3
    message[6] = 1;                     // sensor 4
    message[7] = 1;                     // sensor 5
    generate_checksum(message); // handles inserting checksum into message
    return 1;
}


/*
 * Checks and reads message from uart buffer
 * return 1 for new data
 *        0 for no new data
 *       -1 for fail/error
 */
int hardware_controller::
read_uart_message(std::array<uint8_t, AC_TO_CM_LEN> & message) {
    int rv = 0; 
    std::array<uint8_t, AC_TO_CM_LEN> temp;

    int len = serialDataAvail(uart_fd);
    if(len > 0) { // has new message
        if(len >= AC_TO_CM_LEN) { // full message avaible
            for(unsigned int i=0; i<AC_TO_CM_LEN; ++i)
                message[i] = serialGetchar(uart_fd);
            rv = 1;
        }
        else { // incomplete message
            std::cout << "uart message length " << std::endl; // TODO log this
            rv = -1;
        }
    }
    else if (len == -1) { // error
        std::cout << "serialDataAvail error" << std::endl; // TODO log this
        rv = -1;
    }
    return rv;
}


// store uart data into frame if it is valid
int hardware_controller::
process_uart_message(const std::array<uint8_t, AC_TO_CM_LEN> & message) {
    if(message[0] != '\n')
        return 0;
    if(check_checksum(message) == false) // TODO count this?
        return 0;

    frame.AC_next_failure_mode = message[1];
    frame.AC_failure_mode = message[2];
    frame.AC_failure_cause = message[3];
    frame.AC_consecutive_checksum_errors = message[4];
    frame.AC_lifetime_checksum_errors = message[5];
    frame.AC_sensor1 = message[6];
    frame.AC_sensor2 = message[7];
    frame.AC_sensor3 = message[8];
    frame.AC_sensor4 = message[9];
    std::memcpy(&frame.AC_Sensor5, &message[10], 2);

    return 1;
}


// parses the message's from the Actuator Controller's checksum for correctness
bool hardware_controller::
check_checksum(const std::array<uint8_t, AC_TO_CM_LEN> & message) { 
    uint32_t value = 0;

    for(unsigned int i=0; i<AC_TO_CM_LEN-4; ++i) // dont check 4 checksum bytes
        value += ((message[i] * message[i] * message[i]) % 251);

    uint32_t checksum;
    std::memcpy(&checksum, &message[AC_TO_CM_LEN-4], 4);
    //checksum |= (message[AC_TO_CM_LEN-4] << 24);
    //checksum |= (message[AC_TO_CM_LEN-3] << 16);
    //checksum |= (message[AC_TO_CM_LEN-2] << 8);
    //checksum |= message[AC_TO_CM_LEN-1];

    if(value == checksum)
        return true;
    return false;
}


/* 
 * this will generate a checksum for the message to the Actuator Controller 
 * and insert it into that message
 */
void hardware_controller::
generate_checksum(std::array<uint8_t, CM_TO_AC_LEN> & message) { 
    uint32_t checksum = 0;

    for(unsigned int i=0; i<AC_TO_CM_LEN-4; ++i) // dont check 4 checksum bytes
        checksum += ((message[i] * message[i] * message[i]) % 251);

    std::memcpy(&message[AC_TO_CM_LEN-4], &checksum, 4);

    //message[AC_TO_CM_LEN-4] = (uint8_t)((checksum >> 24) & 0xFF000000);
    //message[AC_TO_CM_LEN-3] = (uint8_t)((checksum >> 16) & 0x00FF0000);
    //message[AC_TO_CM_LEN-2] = (uint8_t)((checksum >> 8) & 0x0000FF00);
    //message[AC_TO_CM_LEN-1] = (uint8_t)(checksum & 0x000000FF);

    return;
}
