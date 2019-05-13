#include "command_control.h"


CommandControl::
CommandControl() {
    CM = std::shared_ptr<CentralManager>(new CentralManager);
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

    std::string command;
    while(1){ //TODO change back to while(1), add end state or break

        // interface output
        command.clear();
        std::cout << std::endl << "Enter a command: ";
        std::cin >> command;
        std::cout << std::endl << std::endl << command << std::endl << std::endl; //keep this
        CM -> input_command(command);
        
        std::this_thread::sleep_for(std::chrono::milliseconds(CC_DELAY));
    }
    return 1;
}
