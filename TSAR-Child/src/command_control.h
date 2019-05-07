#ifndef __CC__
#define __CC__

#include "central_manager.h"
#include "system_status.h"

#include <memory>   // shared ptr

class CommandControl {
    public:
        CommandControl();
        ~CommandControl();
        int start_system();

    private:
        int interface();

        std::shared_ptr<CentralManager> CM;
        std::shared_ptr<SystemStatus> status;
};

#endif 
