#ifndef __STATE__
#define __STATE__

#include <string>       // string
#include <mutex>       // mutex

struct State {
    int current_state;
    std::string last_command;

    int fire_count; // 0 for start up, 0 > for count
    std::mutex state_mutex;

    // TODO add any data needed
};

#endif // __SYSTEM_STATUS__
