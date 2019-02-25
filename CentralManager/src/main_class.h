#ifndef _MAIN_CLASS_
#define _MAIN_CLASS_

#include <iostream>     // cout, cerr
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
        //main_class(const main_class &) = delete;
        //main_class(main_class &&) = delete;
        //main_class& operator=(const main_class &) = delete;
        virtual void driver_loop() = 0;
        void stop_driver();
    protected:
        // gets current time in microseconds as a string
        void get_time_us(std::string & time) const;

        std::atomic<bool> _driver_running;
        std::mutex _mutex;
};
#endif
