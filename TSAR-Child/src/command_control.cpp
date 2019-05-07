#include "command_control.h"

CommandControl::
CommandControl() {
    status = std::shared_ptr<SystemStatus>(new SystemStatus);
    //CM(status);
    // set default state
    // start cm thread?
}

CommandControl::
~CommandControl() {
    // shut down other thread
}

int CommandControl::
start_system() {
    //start CM thred here?

    interface();

    return 1;
}


int CommandControl::
interface() {

    while(1) { //TODO add end state or break
        // interface output
    }

    return 1;
}

