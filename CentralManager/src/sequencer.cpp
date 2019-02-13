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

        hdw_ctrl->get_frame(last_frame);    // update internal sensor data frame

        //if(link != nullptr)
        //    link->recv(new_command);        // get new command from client(s) (if one exist)

        sequence();                         // do system control

        if(link != nullptr)
            link->send(status);             // update clients on progress

        seq_mutex.unlock();
        std::this_thread::sleep_for(std::chrono::microseconds(SEQ_DRIVER_DELAY)); // sleep
    }
    return;
}


// TODO need state diagram
// Where main control theory goes.
// Currently an example of turning a light on and off every second is used.
int sequencer::
sequence() { 
    std::chrono::system_clock::time_point current_time;

    // TODO: add processing of client commands

    unsigned int next_state = status.current_state;

    switch(status.current_state) {
        case 1 : {
            hdw_ctrl->light_on();
            wait_until_time = std::chrono::system_clock::now() + std::chrono::seconds(1);
            next_state = 2;
            break;
        }
        case 2 : {
            current_time = std::chrono::system_clock::now();
            if(current_time >= wait_until_time)
                next_state = 3;
            break;
        }
        case 3 : {
            hdw_ctrl->light_off();
            wait_until_time = std::chrono::system_clock::now() + std::chrono::seconds(1);
            next_state = 4;
            break;
        }
        case 4 : {
            current_time = std::chrono::system_clock::now();
            if(current_time >= wait_until_time)
                next_state = 1;
            break;
        }
        default : {
            // TODO go to emergeny state
            std::cout << "unknown state " << status.current_state << std::endl;
            next_state = 0;
            break;
        }
    }

    status.current_state = next_state;

    return 1; 
}


// TODO: implement this, need state diagram
// check for emergency condistions and will handle them
bool sequencer::
emergency_state() {
    if(status.current_state == 0) {
        std::cout << "emergency_state" << std::endl;
        driver_running = false;
        return true;
    }
    return false;
}


// tells governor to keep thread running or not
bool sequencer::
is_running() {
    seq_mutex.lock();
    return driver_running;
    seq_mutex.unlock();
}
