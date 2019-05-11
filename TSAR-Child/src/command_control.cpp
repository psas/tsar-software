#include "command_control.h"


CommandControl::
CommandControl() {
    
    status = std::shared_ptr<SystemStatus>(new SystemStatus);
    CM = std::shared_ptr<CentralManager>(new CentralManager(status));

    status->current_state = "test";
    status->last_state = "test";
    //CM(status);
    // set default state
    // start cm thread?
     //thread is started from the constructor as the life of the thread needs to be for the life of the object.
    std::thread CM_thread(&CentralManager::CM_loop , CentralManager()); //Creates a thread CM_thread, and sends CM_loop as a function pointer

    status -> current_state.clear(); //clears the strings
    status -> last_state.clear(); // clears the strings
    status -> emergency_stop = false; //by default, not in emergency status
    status -> status_mutex.unlock(); //unlocks the mutex, allowing for write

}


CommandControl::
~CommandControl() {
    // shut down other thread
    std::cout << std::endl << "Exiting CM_thread";
    //CM_thread.join(); //to close out the current thread.
    std::cout << std::endl << "CM_thread exited sucessfully";

    //to reset the states of the strings and mutex lock
    status -> current_state.clear(); //clears the strings
    status -> last_state.clear(); // clears the strings
    status -> emergency_stop = false; //by default, not in emergency status
    status -> status_mutex.unlock(); //unlocks the mutex, allowing for write
}

int CommandControl::
start_system() {
    //start CM thred here?
    CM->CM_loop();

    interface();

    return 1;
}

int CommandControl::
interface() {

    for(unsigned int i=0; i<10; ++i) { //TODO change back to while(1), add end state or break
        std::cout << "CC loop\n";
        // interface output
        std::string command;
        command.clear();
        std::cout << std::endl << "Enter a command: ";
        std::cin >> command;
        status -> current_state = command;

        std::this_thread::sleep_for(std::chrono::milliseconds(CC_DELAY));
    }

    return 1;
}
