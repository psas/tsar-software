#ifndef __SYSTEM_STATUS__
#define __SYSTEM_STATUS__

#include <string>       // string
#include <atomic>       // atomic
#include <mutex>        // mutex

struct SystemStatus {
    std::string current_state;
    std::string last_state;
    std::atomic<bool> emergency_stop;

    // TODO add any data needed by interface display

    std::mutex status_mutex;
};

#endif // __SYSTEM_STATUS__
