#include "hardware_controller.h"


// constructor, need pointer to link
hardware_controller::
hardware_controller(std::shared_ptr<link_logger> & input) : _ll(input), _actuator_controller(UART_PATH, BUAD_RATE), _next_uart_msg(CM_TO_AC_LEN, 0) {
    wiringPiSetup();

    // setup i2c sensors
    _i2c_fds.MPL3115A2_1 = i2c_library::MPL3115A2_setup(MPL3115A2_1_ADD);
    _i2c_data.sensor_1_connected = true;
    _i2c_fds.MPL3115A2_2 = i2c_library::MPL3115A2_setup(MPL3115A2_2_ADD);
    _i2c_data.sensor_2_connected = true;

    // setup gpio pins, since this are built in a vector a enum can be used to index
    _gpio_pins.push_back(pi_gpio(LIGHT_1_GPIO, LOW));
    _gpio_pins.push_back(pi_gpio(LIGHT_2_GPIO, LOW));
    _gpio_pins.push_back(pi_gpio(SEQ_EMERGENCY_LIGHT, LOW));

    // setup uart heartbeat and 1st send message
    _next_heartbeat_time = std::chrono::system_clock::now() - std::chrono::milliseconds(HB_TIME_MS);
    _next_uart_msg[0] = 0xff;
    _next_uart_msg[1] = 0x0f;
}


void hardware_controller::
driver_loop() {
    hardware_data_frame data_frame;

    _driver_running = true;
    while(_driver_running) {
        _mutex.lock();

        update_i2c_data();
        update_gpio_data();
        update_uart_data();

        // update link_logger
        if(_ll != NULL) {
            make_frame(data_frame);
            _ll->send(data_frame);
        }

        _mutex.unlock();
        std::this_thread::sleep_for(std::chrono::microseconds(HDW_DRIVER_DELAY));
    }
    return;
}


// give a copy of data frame
void hardware_controller::
get_frame(hardware_data_frame & input) {
    _mutex.lock();
    make_frame(input);
    _mutex.unlock();
    return;
}


// makes a hardware data frame
void hardware_controller::
make_frame(hardware_data_frame & input) {
    get_time_us(input.time);
    input.i2c_data = _i2c_data;
    input.gpio_data = _gpio_data;
    input.AC_connected = _actuator_controller.status();
    input.AC_data = _AC_data;
    return;
}


// build new sensor frame from live sensors 
void hardware_controller::
update_i2c_data() {
    if(_i2c_data.sensor_1_connected) {
        _i2c_data.pres_1 = i2c_library::MPL3115A2_pres(_i2c_fds.MPL3115A2_1);
        _i2c_data.temp_1 = i2c_library::MPL3115A2_temp(_i2c_fds.MPL3115A2_1);
        if(_i2c_data.pres_1 == -1 || _i2c_data.temp_1 == -1)
            _i2c_data.sensor_1_connected = false;
    }
    if(_i2c_data.sensor_2_connected) {
        _i2c_data.pres_2 = i2c_library::MPL3115A2_pres(_i2c_fds.MPL3115A2_2);
        _i2c_data.temp_2 = i2c_library::MPL3115A2_temp(_i2c_fds.MPL3115A2_2);
        if(_i2c_data.pres_2 == -1 || _i2c_data.temp_2 == -1)
            _i2c_data.sensor_2_connected = false;
    }
    return;
}

// build new sensor frame from live sensors 
void hardware_controller::
update_gpio_data() {
    _gpio_data.light_1_status = _gpio_pins[eLight1].get_status();
    _gpio_data.light_2_status = _gpio_pins[eLight2].get_status();
    return;
}


/* Send uart message if heartbeat timeout has happened and read all incomming data from
 * the Actuator Controller.
 */
void hardware_controller::
update_uart_data() {
    // send hearbeat every HB_TIME_MS milliseconds
    auto current_time = std::chrono::system_clock::now();
    if(current_time >= _next_heartbeat_time) {
        _actuator_controller.send(_next_uart_msg);
        _next_heartbeat_time = current_time + std::chrono::milliseconds(HB_TIME_MS);
    }

    // read message if available
    if(_actuator_controller.status()) {
        std::vector<char> message(AC_TO_CM_LEN, 0);
        if(_actuator_controller.read(message))
            update_AC_data(message); // only if new message
    }
    return;
}


// updates Actuator Controller data
void hardware_controller::
update_AC_data(const std::vector<char> & message) {
    _AC_data.next_failure_mode = message[0];
    _AC_data.failure_mode = message[1];
    _AC_data.failure_cause = message[2];
    _AC_data.consecutive_checksum_errors = message[3];
    _AC_data.lifetime_checksum_errors = message[4];
    _AC_data.sensor1 = message[5];
    _AC_data.sensor2 = message[6];
    _AC_data.sensor3 = message[7];
    _AC_data.sensor4 = message[8];
    std::memcpy(&_AC_data.sensor5, &message[9], sizeof(uint16_t));
    return;
}


// set a gpio pin value, in must be a valid number from gpio enum
void hardware_controller::
set_gpio_value(const int & in, const int & value) {
    _mutex.lock();
    if(in >= 0 && in < eGPIO_COUNT)
        _gpio_pins[in].set_value(value);
    _mutex.unlock();
    return;
}
