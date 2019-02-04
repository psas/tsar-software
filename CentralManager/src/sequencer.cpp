#include "sequencer.h"


// main constructor
sequencer::
sequencer(std::shared_ptr<link_logger> & link_input, std::shared_ptr<hardware_controller> & hdw_ctrl_input) :
        link(link_input), hdw_ctrl(hdw_ctrl_input), main_driver_running(0), 
        high_driver_running(0) {

    status.current_state = 0;
    high_driver_delay.tv_sec = 0;
    high_driver_delay.tv_nsec = (long)SEQ_HIGH_DRIVER_DELAY * 1000000;
    main_driver_delay.tv_sec = 0;
    main_driver_delay.tv_nsec = (long)SEQ_MAIN_DRIVER_DELAY * 1000000;
}


// the high prio sequencer thread driver loop to get data from hardware and check for emergency 
void sequencer::
driver_loop_high() {
    std::cout << "Sequencer High running\n";

    high_driver_running = 1;
    while(high_driver_running == 1) {
        seq_mutex.lock();
        hdw_ctrl->get_frame(last_frame); //update internal frame
        emergency_state();
        seq_mutex.unlock();
        nanosleep(&high_driver_delay, nullptr);
    }
    
    std::cout << "Sequencer High stoped\n";
    
    return;
}


// main thread that handles, main control theory
void sequencer::
driver_loop_main() {
    struct client_command new_command;

    std::cout << "Sequencer Main running\n";

    main_driver_running = 1;
    while(main_driver_running == 1) {
        seq_mutex.lock();
        //if(link->recv(new_command) == 1)
        //    std::cout << "new command\n"; // replace with command stuff later
        sequence(); // control
        link->send(status); // update clients on progress
        seq_mutex.unlock();
        nanosleep(&main_driver_delay, nullptr);
    }
    
    std::cout << "Sequencer Main stoped\n";
    
    return;
}

int sequencer::
sequence() { 
    switch(status.current_state) {
        case 1 :
            hdw_ctrl->light_on();
        default :
            hdw_ctrl->light_off();
    }
    return 1; 
}


void sequencer::
kill_driver() { 
    high_driver_running = 0;
    main_driver_running = 0;
}


int sequencer::
emergency_state() {
    return 1;
}
