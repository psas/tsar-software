#ifndef __STATE__
#define __STATE__

#include <string>       // string
#include <mutex>        // mutex


#define CRACKED 2
#define OPEN 1
#define CLOSED 0
#define ON 1
#define OFF 0

struct State {
    int current_state;
    std::string current_state_name;
    std::string previous_state_name;
    std::string last_command;
    bool in_emergency;

    std::string save_file_name;
    int saving;
    int fire_count; // 0 for start up, 0 > for count
    std::mutex state_mutex;

    bool APC; // Actuator Power Control
    bool VVO; // vent valve oxygen
    bool VVF; // vent valve fuel
    bool OPV; // oxygen pressure valve
    bool FPV; // fuel pressure valve
    bool PPV; // purge pressure valve
    bool IV1; // ignition valve 1
    bool IV2; // ignition valve 2
    int MFV; // main fuel valve
    int MOV; // main oxygen valve. needs 3 states open, closed, cracked
    bool IG; // ignition

    float pressure_1; // Pressure 1
    float pressure_2; // Pressure 2
    float pressure_3; // Pressure 3
    float pressure_4; // Pressure 4

    float temp_1; // Temperature 1
    float temp_2; // Temperature 2
    float temp_3; // Temperature 3
    float temp_4; // Temperature 4
};

#endif // __SYSTEM_STATUS__
