#include "central_manager.h"

CentralManager::
CentralManager() {
    state = std::shared_ptr<State>(new State);
    datafile.open("startup.csv", std::fstream::app); //append to file
    //TODO set i2c reg
    // make state struct
}


CentralManager::
CentralManager(std::shared_ptr<SystemStatus> & input) : status(input) {
    state = std::shared_ptr<State>(new State);
    datafile.open("startup.csv", std::fstream::app); //append to file
    //TODO set i2c reg
    // make state struct
}


CentralManager::
~CentralManager() {
    // TODO may not be needed
    datafile.close();
}


void CentralManager::
CM_loop() {
    for(unsigned int i=0; i<10; ++i) { //TODO change back to while(1), add end state or break
        std::cout << "CM loop" << std::endl;
        read_hardware();
        update();
        state_machine();
        control();
        save();
        std::this_thread::sleep_for(std::chrono::milliseconds(CM_DELAY));
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


int CentralManager::
save() {
    if(!datafile.is_open())
        return 0;

    datafile << status->current_state;
    datafile << ',';
    datafile << status->last_state;
    datafile << '\n';
    
    return 1;
}
