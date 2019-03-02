#include "hardware_controller.h"


// constructor, need pointer to link
hardware_controller::
hardware_controller(std::shared_ptr<link_logger> & input) : _ll(input) {
    wiringPiSetup();

    // setup i2c sensors
    _i2c_fds.MPL3115A2_1 = i2c_library::MPL3115A2_setup(MPL3115A2_1_ADD);
    _i2c_data.sensor_1_connected = true;
    _i2c_fds.MPL3115A2_2 = i2c_library::MPL3115A2_setup(MPL3115A2_2_ADD);
    _i2c_data.sensor_2_connected = true;

    // setup gpio pins
    pinMode(LIGHT_1_GPIO, OUTPUT);
    digitalWrite(LIGHT_1_GPIO, LOW);
    _gpio_data.light_1_status = digitalRead(LIGHT_1_GPIO);
    pinMode(LIGHT_2_GPIO, OUTPUT);
    digitalWrite(LIGHT_2_GPIO, LOW);
    _gpio_data.light_2_status = digitalRead(LIGHT_2_GPIO);

    // setup uart Actuator Controller connection
    _uart_fd = serialOpen(UART_PATH, BUAD_RATE);
    _AC_connected = true;
}

hardware_controller::
~hardware_controller() {
    serialClose(_uart_fd); // close uart connection
}


void hardware_controller::
driver_loop() {
    auto current_time = std::chrono::system_clock::now();
    auto _next_heartbeat_time = current_time - std::chrono::milliseconds(HB_TIME_MS);
    hardware_data_frame data_frame;

    _driver_running = true;
    while(_driver_running) {
        _mutex.lock();

        // update i2c and gpio values
        update_i2c_data();
        update_gpio_data();

        // send uart message
        current_time = std::chrono::system_clock::now();
        if(current_time >= _next_heartbeat_time) {
            uart_library::send_default(_uart_fd); // TODO add function for sequencer to call to change default message
            _next_heartbeat_time = current_time + std::chrono::milliseconds(HB_TIME_MS);
        }

        // read uart message
        if(uart_library::read(_AC_data, _uart_fd) == -1)
            _AC_connected = false;
        else
            _AC_connected = true;
        
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
    input.AC_connected = 1;
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
    _gpio_data.light_1_status = digitalRead(LIGHT_1_GPIO);
    _gpio_data.light_2_status = digitalRead(LIGHT_2_GPIO);
    return;
}


// make gpio pin voltage high
int hardware_controller::
light_on() {
    _mutex.lock();

    digitalWrite(LIGHT_1_GPIO, HIGH);
    _gpio_data.light_1_status = digitalRead(LIGHT_1_GPIO);

    _mutex.unlock();
    return 1;
}


// make gpio pin voltage low
int hardware_controller::
light_off() {
    _mutex.lock();

    digitalWrite(LIGHT_1_GPIO, LOW);
    _gpio_data.light_1_status = digitalRead(LIGHT_1_GPIO);

    _mutex.unlock();
    return 1;
}

