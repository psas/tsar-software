#ifndef __CM__
#define __CM__

#include <memory>   // shared ptr
#include "system_status.h"
#include "state.h"

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

        std::shared_ptr<SystemStatus> status;
};

#endif
