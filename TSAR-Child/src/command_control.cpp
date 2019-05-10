#include "command_control.h"

CommandControl::
CommandControl() {
    status = std::shared_ptr<SystemStatus>(new SystemStatus);
    CM = std::shared_ptr<CentralManager>(new CentralManager(status));

    // set default state
    status->current_state = "test";
    status->last_command = "test";

    // start cm thread?
}


CommandControl::
~CommandControl() {
    // shut down other thread
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

        std::this_thread::sleep_for(std::chrono::milliseconds(CC_DELAY));
    }

    return 1;
}

