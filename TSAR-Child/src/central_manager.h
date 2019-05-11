#ifndef __CM__
#define __CM__

#include <memory>               // shared ptr
#include <fstream>              // file write
#include <iostream>             // io
#include <thread>               // sleep_for
#include <chrono>               // time
#include <string>               // strings
#include <cstring>               // strcmp

#include "system_status.h"
#include "state.h"

#define CM_DELAY                500     // milliseconds
#define IGNITION_START_TIME     100     // milliseconds
#define IGNITION_MAIN_TIME      900     // milliseconds
#define FIRING_STOP_TIME        500     // milliseconds
#define PURGE_TIME              3       // seconds

enum eStates {
    // general
    eStandby,
    eArmed,
    ePreChill,
    eReady,
    ePressurized,
    
    // emergency-stop
    eEmergencyPurge,
    eLockout,
    eSafeShutdown,
    
    // firing sequence
    eIgnitionStart,
    eIgnitionMain,
    eFiring,
    eFiringStop,
    ePurge,
};


class CentralManager {
    public:
        CentralManager(std::shared_ptr<SystemStatus> & input);
        ~CentralManager();
        void CM_loop();

    private:
        int read_hardware();
        int update();
        int state_machine();
        int control();
        int save();
        int parse_fire_command(std::string & command);
        long long get_time_us() const;
        std::string new_file_name();

        long long system_epoch;
        int firetime;
        std::chrono::system_clock::time_point wait_until_time;

        std::shared_ptr<SystemStatus> status;
        std::shared_ptr<State> state;
        std::ofstream datafile;
};

#endif  // __CM__
