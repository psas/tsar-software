#ifndef __CC__
#define __CC__

#include <thread>               // sleep_for
#include <chrono>               // time
#include <memory>               // shared ptr
#include "central_manager.h"
#include <iostream> //printing stuff

#define CC_DELAY 500            // milliseconds


// This class will start the system, make both threads, 
// and handle input from the user.
class CommandControl{
    public:
        CommandControl();
        int start_system();

    private:
        int interface();
        void start_CM_thread();

        std::shared_ptr<CentralManager> CM;
        std::thread CM_thread;
};

#endif  // __CC__
