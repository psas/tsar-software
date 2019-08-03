#ifndef _BASE_CLASS_
#define _BASE_CLASS_

#include <iostream>     // cout, cerr
#include <string>       // string
#include <chrono>       // time
#include <atomic>       // atomic
#include <mutex>        // mutex


// for throwing exceptions in the constructor of base classes
class CM_Exception {
    public: 
        CM_Exception(std::string str) : msg(str) {}
        std::string what() const {return msg;}
    private:
        std::string msg;
};


class base_class {
    public:
        //base_class(const base_class &) = delete;
        //base_class(base_class &&) = delete;
        //base_class& operator=(const base_class &) = delete;
        virtual void driver_loop() = 0;
        void stop_driver();
    protected:
        base_class();
        // gets current time in microseconds as a string
        void get_time_us(std::string & time) const;

        std::atomic<bool> _driver_running;
        std::mutex _mutex;
    private:
        long long _system_epoch;
};
#endif // _BASE_CLASS_
