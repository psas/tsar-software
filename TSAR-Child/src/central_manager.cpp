#include "central_manager.h"

// constructor
CentralManager::
CentralManager() {
    system_epoch = get_time_us();


    // default values in state struct
    state.current_state = 0;
    state.current_state_name = "standby";
    state.APC = OFF;           //Is this correct??
    state.save_file_name = "";
    state.fire_count =0;

    // gpio
    gp = GPIO::GPIOManager::getInstance();
    APC_fd = GPIO::GPIOConst::getInstance()->getGpioByKey(APC_PIN);
    VVO_fd = GPIO::GPIOConst::getInstance()->getGpioByKey(VVO_PIN);
    VVF_fd = GPIO::GPIOConst::getInstance()->getGpioByKey(VVF_PIN);
    OPV_fd = GPIO::GPIOConst::getInstance()->getGpioByKey(OPV_PIN);
    FPV_fd = GPIO::GPIOConst::getInstance()->getGpioByKey(FPV_PIN);
    PPV_fd = GPIO::GPIOConst::getInstance()->getGpioByKey(PPV_PIN);
    IV1_fd = GPIO::GPIOConst::getInstance()->getGpioByKey(IV1_PIN);
    IV2_fd = GPIO::GPIOConst::getInstance()->getGpioByKey(IV2_PIN);
    MFV0_fd = GPIO::GPIOConst::getInstance()->getGpioByKey(MFV0_PIN);
    MFV1_fd = GPIO::GPIOConst::getInstance()->getGpioByKey(MFV1_PIN);
    MOV0_fd = GPIO::GPIOConst::getInstance()->getGpioByKey(MOV0_PIN);
    MOV1_fd = GPIO::GPIOConst::getInstance()->getGpioByKey(MOV1_PIN);
    IG_fd = GPIO::GPIOConst::getInstance()->getGpioByKey(IG_PIN);
    gp->setDirection(APC_fd, GPIO::OUTPUT);
    gp->setDirection(VVO_fd, GPIO::OUTPUT);
    gp->setDirection(VVF_fd, GPIO::OUTPUT);
    gp->setDirection(OPV_fd, GPIO::OUTPUT);
    gp->setDirection(FPV_fd, GPIO::OUTPUT);
    gp->setDirection(PPV_fd, GPIO::OUTPUT);
    gp->setDirection(IV1_fd, GPIO::OUTPUT);
    gp->setDirection(IV2_fd, GPIO::OUTPUT);
    gp->setDirection(MFV0_fd, GPIO::OUTPUT);
    gp->setDirection(MFV1_fd, GPIO::OUTPUT);
    gp->setDirection(MOV0_fd, GPIO::OUTPUT);
    gp->setDirection(MOV1_fd, GPIO::OUTPUT);
    gp->setDirection(IG_fd, GPIO::OUTPUT);

    //TODO: initialize data acquisition class
    //
    //TODO: Call data acquisition hooks, populate state struct with data
}


// deconstructor
CentralManager::
~CentralManager() {
    if(datafile.is_open())
        datafile.close();

    if(gp != nullptr) {
        delete gp;
        gp = nullptr;
    }
}

// main loop for central manager, that call the main functions
void CentralManager::
CM_loop() {
    safe_state_zero();
    state.previous_state_name = "pre-start"; //made-up state name so
    					     //the transition function works

    while(1){ // TODO add end state or break
        state.state_mutex.lock();
        read_hardware();
        check_for_emergency();
        state_machine();
        //control();
        state.saving = save();
        state.state_mutex.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(CM_DELAY));
    }
}

// Handle reading all the I2C sensors
int CentralManager::
read_hardware() {
    return 1;
}

int CentralManager::
check_for_emergency() {
    // TODO add this later
    //
    // NOTE: this function is for emergency auto detect
    // the emergency stop command is handled by the state machine function
    return 1;
}

int CentralManager::
emergency() {
    state.in_emergency = true;
    if(state.current_state == eStandby) {
        state.current_state = eLockout;
        state.current_state_name = "lockout";
    }
    else {
        state.current_state = eEmergencyPurge;
        state.current_state_name = "emergency purge";
    }
    return 0;
}

// If the tranistion requriment(s) are met, the settings for the next state
// are set as it swap to the next state.
int CentralManager::
state_machine() {
    if(state.current_state != state.previous_state) {
        switch(state.current_state){
            case eStandby:
                state.current_state_name = "standby";
                // TODO: stuff
                break;
            default:
                state.current_state = eEmergencyPurge;
                state.current_state_name = "emergency purge";
        }
    }

    state.previous_state = state.current_state;
    state.previous_state_name = state.current_state_name;
}

/*
* TODO: remove this
*
int CentralManager::
transition_state(const int from, const int to) {
	state.current_state = to;
	switch(to) {
		case eStandby: {
	        	safe_state_zero();
                	state.APC = OFF;
		}
		case eReady: {
			//check if file is open adn if so close it
			//TO OPEN DATA/STARTUP.CSV
			break;
		}
		case ePressurized: {
			//check if file is open, if so close it
			//open new file with new unused log name
			break;
		}


	}
}*/


// Handles controlling a valve
int CentralManager::
control_valve(const bool & valve, const int & fd) {
    if(valve == OPEN)
        gp->setValue(fd, GPIO::HIGH);
    else if(valve == CLOSED)
        gp->setValue(fd, GPIO::LOW);
    return 1;
}

// Handles controlling all the valves
int CentralManager::
control() {
    control_valve(state.VVO, VVO_fd);
    control_valve(state.VVF, VVF_fd);
    control_valve(state.OPV, OPV_fd);
    control_valve(state.FPV, FPV_fd);
    control_valve(state.PPV, PPV_fd);
    control_valve(state.IV1, IV1_fd);
    control_valve(state.IV2, IV2_fd);
    if(state.MFV == OPEN) {
	gp->setValue(MFV0_fd, GPIO::HIGH);
        gp->setValue(MFV1_fd, GPIO::HIGH);
    }
    else if(state.MFV == CRACKED) {
	gp->setValue(MFV0_fd, GPIO::HIGH);
	gp->setValue(MFV1_fd, GPIO::LOW);
    }
    else if(state.MFV == CLOSED) {
	gp->setValue(MOV0_fd, GPIO::LOW);
	gp->setValue(MOV1_fd, GPIO::LOW);
    }
    if(state.MOV == OPEN) {
	gp->setValue(MOV0_fd, GPIO::HIGH);
        gp->setValue(MOV1_fd, GPIO::HIGH);
    }
    else if(state.MOV == CRACKED) {
	gp->setValue(MOV0_fd, GPIO::HIGH);
	gp->setValue(MOV1_fd, GPIO::LOW);
    }
    else if(state.MOV == CLOSED) {
	gp->setValue(MOV0_fd, GPIO::LOW);
	gp->setValue(MOV1_fd, GPIO::LOW);
    }
    if(state.APC == ON)
	gp->setValue(APC_fd, GPIO::HIGH);
    else if(state.APC == OFF)
	gp->setValue(APC_fd, GPIO::LOW);
    if(state.IG ==  ON)
        gp->setValue(IG_fd, GPIO::HIGH);
    else if(state.IG == OFF)
        gp->setValue(IG_fd, GPIO::LOW);

    return 1;
}

// Handle reading all the I2C sensors
void CentralManager::
safe_state_zero() {
    state.APC = ON;
    state.VVO = OPEN;
    state.VVF = OPEN;
    state.OPV = CLOSED;
    state.FPV = CLOSED;
    state.PPV = CLOSED;
    state.IV1 = CLOSED;
    state.IV2 = CLOSED;
    state.MFV = CLOSED;
    state.MOV = CLOSED;
    state.IG = OFF;
    return;
}



// save all the data in the state struct
int CentralManager::
save() {
    if(state.current_state == eStandby || state.current_state == eSafeShutdown) {
        return 0;
    }
    if(!datafile.is_open()) {
        return -1;
    }

    // TODO add data here
    datafile << (get_time_us() - system_epoch);
    datafile << ',';
    datafile << state.current_state_name;
    datafile << ',';
    datafile << state.fire_count;
    datafile << '\n';
    datafile.flush();

    return 1;
}


// expects a command input of "fire x" where x is any number of any length
// will return x as a int, if greater than 1 second
// returns -1 if fails
int CentralManager::
parse_fire_command(std::string & command) {
    int time_int;

    // create new string with "fire " remove from input string
    std::string time = command.substr(strlen("fire"), command.length() - strlen("fire"));

    try {
        time_int = std::stoi(time);
        if (time_int < 1) {
            time_int = -1;
        }
    } catch(...) {
        time_int = -1;
    }

    return time_int;
}


// get the current time as a long long to save
long long CentralManager::
get_time_us() const {
    auto now = std::chrono::steady_clock::now();
    auto now_us = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
    auto epoch = now_us.time_since_epoch();
    auto value = std::chrono::duration_cast<std::chrono::milliseconds>(epoch);
    return value.count();
}


// make new name for file with the format "fireX.csv", where X is the current fire count
std::string CentralManager::
new_file_name() {
    std::string file = "data/fire";
    file.append(std::to_string(state.fire_count));
    file.append(".csv");
    return file;
}


// allows the command and control thread to set input the last command
int CentralManager::
input_command(std::string &command){
    state.state_mutex.lock();
    state.last_command = command;
    state.state_mutex.unlock();
    return 0;
}


// allows the command and control thread to get a copy of the data
int CentralManager::
copy_data(State & input) {
    state.state_mutex.lock();
    std::memcpy(&input, &state, sizeof(state));
    state.state_mutex.unlock();
    return 0;
}
