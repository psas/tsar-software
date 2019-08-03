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
    std::string last_command;
    bool in_emergency;

    std::string save_file_name;
    int saving;
    int fire_count; // 0 for start up, 0 > for count
    std::mutex state_mutex;

    bool VVO; // vent valve oxygen
    bool VVF; // vent valve fuel
    bool OPV; // oxygen pressure valve
    bool FPV; // fuel pressure valve
    bool PPV; // purge pressure valve
    bool IV1; // ignition valve 1
    bool IV2; // ignition valve 2
    bool MFV; // main fuel valve
    int MOV; // main oxygen valve. needs 3 states open, closed, cracked
    bool IG; // ignition

    // TODO add any data needed
    int generic_unamed_dat_that_i_dont_know_what_to_call[16];
};

#endif // __SYSTEM_STATUS__
