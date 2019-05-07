#ifndef __CC__
#define __CC__

#include "system_status.h"
#include "central_manager.h"
#include "system_status.h"
#include <thread> //for multithreading
#include <iostream> //printing stuff

class CommandControl{
    public:
        CommandControl();
        ~CommandControl();
        int start_system();

    private:
        int interface();
        
        SystemStatus status;
};

#endif 
