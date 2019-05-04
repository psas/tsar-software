#include "command_control.h"
#include "central_manager.h"

CommandControl::
CommandControl() {
    // set default state
    // start cm thread
}

CommandControl::
~CommandControl() {
    // shut down other thread
}

int CommandControl::
start_system() {
    //start CM thred

    interface();

    return 1;
}


void CommandControl::
inferface() {

    while(1) { //TODO add end state or break
        // interface output
    }

    return;
}

