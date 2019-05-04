#include "central_manager.h"

CentralManager::
CentralManager() {
    //TODO set i2c reg
}


CentralManager::
~CentralManager() {
    // TODO may not be needed
}


void CentralManager::
CM_loop() {
    while(1) { //TODO add end state or break
        read_hardware();
        update();
        state_machine();
        control();
    }
}


int CentralManager::
read_hardware() {}


int CentralManager::
update() {}


int CentralManager::
state_machine() {}


//TODO
int CentralManager::
control() { 
    std::chrono::system_clock::time_point current_time;

    status.current_state = status.next_state;

    switch(status.current_state) {
        case eEnd:
            break;
        case eStart:
            break;
        case eHalt:
            break;
        case eWait: 
            break;
        case eLightOn:
            break;
        case eLightOff:
            break;
        default :
            break;
    }
    return 1; 
}
