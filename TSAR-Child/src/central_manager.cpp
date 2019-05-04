#include "central_manager.h"


void CentralManager::
driver_loop() {
    while(1) { //TODO add end state or break
        read_hardware();
        update();
        sequence();
        control();
        // sleep
    }

}




// Where main control theory goes.
// Currently an example of turning a light on and off every second is used.
int Control::
sequence() { 
    std::chrono::system_clock::time_point current_time;

    status.current_state = status.next_state;

    switch(status.current_state) {
        case eEnd: // will shut down system
            //TODO do this
            status.next_state = eEnd;
            break;
        case eStart: // wait for start command
            status.next_state = eLightOn; // TODO for non sequence test
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
            hdw_ctrl->set_gpio_value(eLight1, HIGH);
            wait_until_time = std::chrono::system_clock::now() + std::chrono::seconds(1);
            status.next_state = eWait;
            status.state_after_wait = eLightOff;
            break;
        case eLightOff:
            hdw_ctrl->set_gpio_value(eLight1, LOW);
            wait_until_time = std::chrono::system_clock::now() + std::chrono::seconds(1);
            status.next_state = eWait;
            status.state_after_wait = eLightOn;
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
bool Control::
emergency_state() { // call emergency hardware functions, TODO make emergency hardware functions
    bool emergency;

    if(status.current_state == eEmergency) {
        if(last_hdw_frame.AC_connected == true) {
            status.next_state = status.state_after_emergency;
            hdw_ctrl->set_gpio_value(eEmergencyLight, LOW);
            emergency = false;
        }
        else
            // call emergency function(s)
            emergency = true;
    }
    else{
        if(last_hdw_frame.AC_connected == false) {
            status.current_state = eEmergency;
            status.state_after_emergency = status.next_state;
            status.next_state = eEmergency;
            hdw_ctrl->set_gpio_value(eEmergencyLight, HIGH);
            // call emergency function(s)
            emergency = true;
        }
        else
            emergency = false;
    }

    return emergency;
}

