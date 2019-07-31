#ifndef __CM__
#define __CM__

#include <fstream>              // file write
#include <iostream>             // io
#include <thread>               // sleep_for
#include <chrono>               // time
#include <string>               // strings
#include <cstring>               // strcmp

#include "state.h"
#include "GPIO/GPIOManager.h"
#include "GPIO/GPIOConst.h"

#define FILE_HEADER "time(ms), state, fire_count\n"
#define CM_DELAY                100     // milliseconds
#define PRINT_DELAY             500     // milliseconds
#define IGNITION_START_TIME     100     // milliseconds
#define IGNITION_OXIDIZE_TIME   500     // milliseconds
#define IGNITION_MAIN_TIME      900     // milliseconds
#define FIRING_STOP_TIME        500     // milliseconds
#define PURGE_TIME              3       // seconds
#define EMERGENCY_SAFE_TIME     10      // seconds 

#define VVO_PIN 		"P8_7"
#define VVF_PIN 		"P8_8"
#define OPV_PIN			"P8_9"
#define FPV_PIN 		"P8_10"
#define PPV_PIN 		"P8_11"
#define IV1_PIN 		"P8_12"
#define IV2_PIN 		"P8_14"
#define MFV_PIN 		"P8_15" // TODO change to UART later
#define MOV_PIN 		"P8_16"
#define IG_PIN 			"P8_17"

enum eStates {
    // general
    eStandby,
    eArmed,
    ePreChill,
    eReady,
    ePressurized,
    
    // emergency-stop
    eEmergencyPurge,
    eEmergencySafe,
    eLockout,
    eSafeShutdown,
    
    // firing sequence
    eIgnitionStart,
    eIgnitionOxidize,
    eIgnitionMain,
    eFiring,
    eFiringStop,
    ePurge,
};

// This class handles most of the hard work of controlling the system.
// Handles reading/controlling hardware, and controls the state of the system.
class CentralManager {
    public:
        CentralManager();
        ~CentralManager();
        void CM_loop();
        int input_command(std::string &command);
        int copy_data(State & input);
        int emergency();

    private:
        int read_hardware();
        int check_for_emergency();
        int state_machine();
        int control();
        int save();
        int parse_fire_command(std::string & command);
        long long get_time_us() const;
        std::string new_file_name();
        int control_valve(const bool & valve, const int & fd);
        void valve_safe_state();

        long long system_epoch;
        int firetime;
        std::chrono::system_clock::time_point wait_until_time;
        State state;
        std::ofstream datafile;

        // GPIO stuff
        GPIO::GPIOManager * gp;
        int VVO_fd;
        int VVF_fd;
        int OPV_fd;
        int FPV_fd;
        int PPV_fd;
        int IV1_fd;
        int IV2_fd;
        int MFV_fd;
        int MOV_fd;
        int IG_fd;
};

#endif  // __CM__
