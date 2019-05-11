#ifndef __CM__
#define __CM__

#include <memory>               // shared ptr
#include <fstream>              // file write
#include <iostream>             // io
#include <thread>               // sleep_for
#include <chrono>               // time

#include "system_status.h"
#include "state.h"

#define CM_DELAY 500            // milliseconds

class CentralManager {
    public:
        CentralManager();
        CentralManager(std::shared_ptr<SystemStatus> & input);
        ~CentralManager();
        void CM_loop();

    private:
        int read_hardware();
        int update();
        int state_machine();
        int control();
        int save();

        std::shared_ptr<SystemStatus> status;
        std::shared_ptr<State> state;
        std::ofstream datafile;
};

#endif  // __CM__
