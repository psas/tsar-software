#include "sequencer.h"

// main constructor
sequencer::
sequencer(std::shared_ptr<link_logger> & link_input, std::shared_ptr<hardware_controller> & hdw_ctrl_input) : 
        link(link_input), hdw_ctrl(hdw_ctrl_input) {
    status.current_state = eStart;
    status.next_state = eStart;
}


// main thread that handles, main control theory
void sequencer::
driver_loop() {
    client_command new_command;

    _driver_running = true;
    while(_driver_running) {
        _mutex.lock();

        // check for emergency and deal with them if found
        if(emergency_state()) {
            _mutex.unlock();
            continue;// restart loop if emergency was found
        }

        // update internal sensor data frame
        hdw_ctrl->get_frame(last_hdw_frame);

        // process any commands from clients
        if(link != nullptr)
            if(link->recv(new_command))
                process_command(new_command);

        sequence();

        // update clients
        if(link != nullptr) {
            get_time_us(status.time);
            link->send(status);
        }

        _mutex.unlock();
        std::this_thread::sleep_for(std::chrono::microseconds(SEQ_DRIVER_DELAY)); // sleep
    }
    return;
}


bool sequencer::
process_command(const client_command & data) {
    bool command_processed = true;
    std::cout << "Seq is processing command" << std::endl;
    if(data.seq_command == "stop") {
        status.current_state = eStop;
    }
    else if(data.seq_command == "halt") {
        status.state_after_halt = status.current_state;
        status.current_state = eHalt;
    }
    else if(data.seq_command == "continue" && status.current_state == eHalt) {
        status.current_state = status.state_after_halt;
    }
    else if(data.seq_command == "start") {
        status.current_state = eLightOn;
    }
    else if(data.seq_command == "end" && status.current_state == eStop) {
        status.current_state = eEnd;
    }
    else {
        std::cout << "unknowned command processed" << std::endl;
        command_processed = false;
    }

    return command_processed;
}


// TODO need state diagram
// Where main control theory goes.
// Currently an example of turning a light on and off every second is used.
int sequencer::
sequence() { 
    std::chrono::system_clock::time_point current_time;

    status.current_state = status.next_state;

    switch(status.current_state) {
        case eEnd: // will shut down system
            //TODO do this
            status.next_state = eEnd;
            break;
        case eStart: // wait for start command
            status.next_state = eStart;
            break;
        case eHalt:
            status.next_state = eHalt;
            break;
        case eWait: 
            current_time = std::chrono::system_clock::now();
            if(current_time >= wait_until_time)
                status.next_state = status.state_after_wait;
            else
                status.next_state = status.current_state;
            break;
        case eLightOn:
            hdw_ctrl->light_on();
            wait_until_time = std::chrono::system_clock::now() + std::chrono::seconds(1);
            status.next_state = eWait;
            status.state_after_wait = eLightOff;
            break;
        case eLightOff:
            hdw_ctrl->light_off();
            wait_until_time = std::chrono::system_clock::now() + std::chrono::seconds(1);
            status.next_state = eWait;
            status.state_after_wait = eLightOff;
            break;
        default :
            std::cout << "unknown state " << std::endl;
            status.next_state = eUnknown;
            break;
    }
    return 1; 
}


/* Checks for emergency condistions and will handle them by calling hardware emergency functions
 *      false was not in emergency state
 *      true was in emergency state
 */
bool sequencer::
emergency_state() {
    if(status.current_state == eUnknown || status.current_state == eEmergency) {
        std::cout << "emergency_state" << std::endl;
        status.current_state = eEmergency;
        // call emergency hardware functions, TODO make emergency hardware functions
        return true;
    }
    return false;
}


// tells governor to keep thread running or not
bool sequencer::
is_running() {
    _mutex.lock();
    bool r = _driver_running;
    _mutex.unlock();
    return r;
}
