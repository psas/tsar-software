#include "command_control.h"
#include "central_manager.h"



#include <thread> //for multithreading
CommandControl::CommandControl(){
    // set default state
    // start cm thread
    
    //thread is started from the constructor as the life of the thread needs to be for the life of the object.
    std::thread CM_thread(&CentralManager::CM_loop , CentralManager()); //Creates a thread CM_thread, and sends CM_loop as a function pointer

    status.current_state.clear(); //clears the strings
    status.last_state.clear(); // clears the strings
    status.emergency_stop = false; //by default, not in emergency status
    status.status_mutex.unlock(); //unlocks the mutex, allowing for write
}

CommandControl::~CommandControl() {
    // shut down other thread
    std::cout << std::endl << "Exiting CM_thread";
    //CM_thread.join(); //to close out the current thread.
    std::cout << std::endl << "CM_thread exited sucessfully";

    //to reset the states of the strings and mutex lock
    status.current_state.clear(); //clears the strings
    status.last_state.clear(); // clears the strings
    status.emergency_stop = false; //by default, not in emergency status
    status.status_mutex.unlock(); //unlocks the mutex, allowing for write
}

int CommandControl::start_system(){

    interface();

    return 1;
}

int CommandControl::interface() {

    while(1) { //TODO add end state or break
        // interface output

    }

    return 0;
}
