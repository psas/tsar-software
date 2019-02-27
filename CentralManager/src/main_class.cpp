#include "main_class.h"

main_class::
main_class() : _driver_running(false) {
    auto now = std::chrono::steady_clock::now();
    auto now_us = std::chrono::time_point_cast<std::chrono::microseconds>(now);
    auto epoch = now_us.time_since_epoch();
    auto value = std::chrono::duration_cast<std::chrono::microseconds>(epoch);
    _system_epoch = value.count();
}

/*
 * gets monotonic time from hardware (in microseconds) as a string
 * used for logging and data frames sent to clients
 */
void main_class::
get_time_us(std::string & time) const {
    auto now = std::chrono::steady_clock::now();
    auto now_us = std::chrono::time_point_cast<std::chrono::microseconds>(now);
    auto epoch = now_us.time_since_epoch();
    auto value = std::chrono::duration_cast<std::chrono::microseconds>(epoch);
    long long duration = value.count() - _system_epoch;
    time = std::to_string(duration);
}


void main_class::
stop_driver() {
    _mutex.lock();
    _driver_running = false;
    _mutex.unlock();
}
