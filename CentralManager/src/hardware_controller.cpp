#include "hardware_controller.h"


// constructor, need pointer to link
hardware_controller::
hardware_controller(std::shared_ptr<link_logger> & input) : main_class(), ll(input) noexcept {
    wiringPiSetup()

    // setup uart
    uart_fd = serialOpen(UART_PATH, BUAD_RATE);

    // setup hardware sensors
    _fd_list.MPL3115A2_1 = hardware_library::MPL3115A2_setup(MPL3115A2_1_ADD);
    _fd_list.MPL3115A2_2 = hardware_library::MPL3115A2_setup(MPL3115A2_2_ADD);

    // setup gpio pins
    pinMode(LIGHT_GPIO, OUTPUT);
    digitalWrite(LIGHT_GPIO, LOW);  // init value (LOW/HIGH)
    _frame.light_status = false;     // init state
}

hardware_controller::
~hardware_controller() {
    serialClose(_uart_fd);
}


void hardware_controller::
driver_loop() {
    std::chrono::system_clock::time_point current_time;

    _driver_running = true;
    while(_driver_running) {
        _mutex.lock();

        update_frame();

        hardware_library::uart_update(_frame.AC_data, _uart_fd); // TODO check for heartbeat

        current_time = std::chrono::system_clock::now();
        if(current_time >= _next_heartbeat_time) {
            hardware_library::uart_send_command(_uart_fd); // TODO define this as non default commands
            _next_heartbeat_time = current_time + std::chrono::milliseconds(10);
        }

        if(_ll != NULL)
            _ll->send(_frame);

        _mutex.unlock();
        std::this_thread::sleep_for(std::chrono::microseconds(HDW_DRIVER_DELAY));
    }
    return;
}



// give a copy of data frame
int hardware_controller::
get_frame(sensor_data_frame & input) {
    _mutex.lock();
    input = _frame;
    _mutex.unlock();
    return 1;
}


// build new sensor frame from live sensors 
int hardware_controller::
update_frame() {
    get_time_us(_frame.time);

    // TODO deal with sensor deconnecting
    _frame.pres_1 = hardware_library::MPL3115A2_pres(_fd_list.MPL3115A2_1);
    _frame.temp_1 = hardware_library::MPL3115A2_temp(_fd_list.MPL3115A2_1);
    _frame.pres_2 = hardware_library::MPL3115A2_pres(_fd_list.MPL3115A2_2);
    _frame.temp_2 = hardware_library::MPL3115A2_temp(_fd_list.MPL3115A2_2);

    return 1;
}


// make gpio pin voltage high
int hardware_controller::
light_on() {
    _mutex.lock();

    digitalWrite(LIGHT_GPIO, HIGH);
    _frame.light_status = true;

    _mutex.unlock();
    return 1;
}


// make gpio pin voltage low
int hardware_controller::
light_off() {
    _mutex.lock();

    digitalWrite(LIGHT_GPIO, LOW);
    _frame.light_status = false;

    _mutex.unlock();
    return 1;
}

