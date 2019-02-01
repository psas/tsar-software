#include "sequencer.h"


// main constructor
sequencer::
sequencer(link_logger * link_input, hardware_controller * hdw_ctrl_input)
        : link(link_input), hdw_ctrl(hdw_ctrl_input), main_driver_running(0), 
        high_driver_running(0) {

    status.current_state = 0;
    high_driver_delay.tv_sec = 0;
    high_driver_delay.tv_nsec = (long)SEQ_HIGH_DRIVER_DELAY * 1000000;
    main_driver_delay.tv_sec = 0;
    main_driver_delay.tv_nsec = (long)SEQ_MAIN_DRIVER_DELAY * 1000000;
    status_size = sizeof(struct sequence_status);
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

#ifdef LIVE_DATA
int sequencer::
sequence() { 
    cout << "sequence\n";
    // add switch statment here
}
#else // for testing/debug
int sequencer::
sequence() { 
    switch(status.current_state) {
        case 1 :
            hdw_ctrl->light_on();
            if(last_frame.test_int_0 > 80)
                status.current_state = 2;
            else
                break;
        default :
            hdw_ctrl->light_off();
            if(last_frame.test_int_1 < 80)
                status.current_state = 1;
            break;
    }
    return 1; 
}
#endif


void sequencer::
kill_driver() { 
    high_driver_running = 0;
    main_driver_running = 0;
}


int sequencer::
emergency_state() {
    if(last_frame.test_int_1%20 == 1) { // placeholder for later logic
        //std::cout << "EMERGENCY HALT\n";
        status.current_state = -1;
        link->send(status);
        return 1;
    }
    return 0;
}
