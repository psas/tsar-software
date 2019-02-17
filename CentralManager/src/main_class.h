#ifndef _MAIN_CLASS_
#define _MAIN_CLASS_

#include <iostream>     // cout
#include <string>       // string
#include <chrono>       // time
#include <atomic>       // atomic
#include <mutex>        // mutex


// for throwing exceptions in the constructor of main classes
class CM_Exception {
    public: 
        CM_Exception(std::string str) : msg(str) {}
        std::string what() const {return msg;}
    private:
        std::string msg;
};


class main_class {
    public:
        virtual void driver_loop() = 0;
    private:
        // gets current time in microseconds as a string
        void get_time_us(std::string & time) const;

        std::atomic<bool> driver_running;
        std::mutex hdw_mutex;
};
#endif
