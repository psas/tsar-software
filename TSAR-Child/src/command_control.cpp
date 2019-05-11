#include "command_control.h"


CommandControl::
CommandControl() {
    
    status = std::shared_ptr<SystemStatus>(new SystemStatus);
    CM = std::shared_ptr<CentralManager>(new CentralManager(status));

    // set default state
    status->current_state = "test";
    status->last_command = "test";

    // start cm thread?
     //thread is started from the constructor as the life of the thread needs to be for the life of the object.
    status -> status_mutex.lock(); //locks the mutex, allowing for write
    status -> current_state.clear(); //clears the strings
    status -> last_command.clear(); // clears the strings
    status -> emergency_stop = false; //by default, not in emergency status
    status -> status_mutex.unlock(); //unlocks the mutex, allowing for write
}


CommandControl::
~CommandControl() {
    // shut down other thread
    //CM_thread.join(); //to close out the current thread.

    //to reset the states of the strings and mutex lock

    status -> status_mutex.lock(); //locks the mutex, allowing for write
    status -> current_state.clear(); //clears the strings
    status -> last_command.clear(); // clears the strings
    status -> emergency_stop = false; //by default, not in emergency status
    status -> status_mutex.unlock(); //unlocks the mutex, allowing for write
}

int CommandControl::
start_system(){
    //start CM thred here?

    std::thread CM_thread(&CommandControl::start_CM_thread , this); //Creates a thread CM_thread, and sends CM_loop as a function pointer
    
    interface();
    
    if(CM_thread.joinable())
        CM_thread.join();

    return 1;
}
void CommandControl::start_CM_thread(){
    CM->CM_loop();
    return;
}

int CommandControl::
interface() {

    while(1){ //TODO change back to while(1), add end state or break

        // interface output
        std::string command;
        command.clear();
        std::cout << std::endl << "Enter a command: ";
        std::cin >> command;
        std::cout << std::endl << command << std::endl << std::endl;
        status -> current_state = command;
        
        std::this_thread::sleep_for(std::chrono::milliseconds(CC_DELAY));
    }
    return 1;
}
