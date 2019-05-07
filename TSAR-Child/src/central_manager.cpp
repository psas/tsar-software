#include "central_manager.h"


CentralManager::
CentralManager(std::shared_ptr<SystemStatus> & input) : status(input) {
    //TODO set i2c reg
    // make state struct
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
read_hardware() {
    return 1;
}


int CentralManager::
update() {
    return 1;
}


int CentralManager::
state_machine() {
    return 1;
}


//TODO
int CentralManager::
control() { 
    return 1; 
}
