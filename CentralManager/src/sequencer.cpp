#include "sequencer.h"

// main constructor
sequencer::
sequencer(std::shared_ptr<link_logger> & link_input, std::shared_ptr<hardware_controller> & hdw_ctrl_input) : 
        link(link_input), hdw_ctrl(hdw_ctrl_input), driver_running(false) {
    status.current_state = 1;
}

// main thread that handles, main control theory
void sequencer::
driver_loop() {
    client_command new_command;

    driver_running = true;
    while(driver_running) {
        seq_mutex.lock();
        if(emergency_state())               // check for emergency and deal with them
            continue;                       // restart loop if emergency was found
        hdw_ctrl->get_frame(last_frame);    // update internal frame
        link->recv(new_command);            // get new command from client(s) (if one exist)
        sequence();                         // do system control
        link->send(status);                 // update clients on progress
        seq_mutex.unlock();
        std::this_thread::sleep_for(std::chrono::microseconds(SEQ_DRIVER_DELAY)); // sleep
    }
    return;
}


// Where main control theory goes.
// Currently an example of turning a light on and off every second is used.
int sequencer::
sequence() { 
    std::chrono::system_clock::time_point current_time;

    // TODO: add processing of client commands

    switch(status.current_state) {
        case 1 : {
            hdw_ctrl->light_on();
            wait_until_time = std::chrono::system_clock::now() + std::chrono::seconds(1);
        }
        case 2 : {
            current_time = std::chrono::system_clock::now();
            if(current_time >= wait_until_time)
                next_state = 3;
            else
                break;
        }
        case 3 : {
            hdw_ctrl->light_off();
            wait_until_time = std::chrono::system_clock::now() + std::chrono::seconds(1);
        }
        case 4 : {
            current_time = std::chrono::system_clock::now();
            if(current_time >= wait_until_time)
                next_state = 1;
            else
                break;
        }
        default :
            driver_running = false;
            break;
    }

    status.current_state = next_state;

    return 1; 
}


// TODO: implement this
// check for emergency condistions and will handle them
bool sequencer::
emergency_state() {
    if(status.current_state == 0)
        return true;
    return false;
}

// tells governor to keep thread running or not
bool sequencer::
is_running() {
    return driver_running;
}
