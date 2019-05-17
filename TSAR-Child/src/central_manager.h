#ifndef __CM__
#define __CM__

#include <memory>               // shared ptr
#include <fstream>              // file write
#include <iostream>             // io
#include <thread>               // sleep_for
#include <chrono>               // time
#include <string>               // strings
#include <cstring>               // strcmp

#include "state.h"
#include "GPIO/GPIOManager.h"
#include "GPIO/GPIOConst.h"

#define CM_DELAY                100     // milliseconds
#define PRINT_DELAY             500     // milliseconds
#define IGNITION_START_TIME     100     // milliseconds
#define IGNITION_MAIN_TIME      900     // milliseconds
#define FIRING_STOP_TIME        500     // milliseconds
#define PURGE_TIME              3       // seconds

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

#define TEXT_WHITE              "\033[0m"
#define TEXT_RED                "\033[31m"
#define TEXT_YELLOW             "\033[33m"

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

// This class handles most of the hard work of controlling the system.
// Handles reading/controlling hardware, controls the state of the system, 
// and updates the user on progress.
class CentralManager {
    public:
        CentralManager();
        ~CentralManager();
        void CM_loop();
        int input_command(std::string &command);

    private:
        int read_hardware();
        int update();
        int state_machine();
        int control();
        int save();
        int parse_fire_command(std::string & command);
        long long get_time_us() const;
        std::string new_file_name();
        void print_input_error(std::string & command, std::string & state);

        long long system_epoch;
        int firetime;
        std::chrono::system_clock::time_point wait_until_time;
        std::chrono::system_clock::time_point next_print_time;

        std::shared_ptr<State> state;
        std::ofstream datafile;
        GPIO::GPIOManager * gp;
        int VVO_pin;
        int VVF_pin;
        int OPV_pin;
        int FPV_pin;
        int PPV_pin;
        int IV1_pin;
        int IV2_pin;
        int MFV_pin;
        int MOV_pin;
        int IG_pin;
};

#endif  // __CM__
