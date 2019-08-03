#include "central_manager.h"

// constructor
CentralManager::
CentralManager() {
    system_epoch = get_time_us();

    // make new data foldN folders

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
    MFV_fd = GPIO::GPIOConst::getInstance()->getGpioByKey(MFV_PIN); // TODO swap to uart later
    MOV_fd = GPIO::GPIOConst::getInstance()->getGpioByKey(MOV_PIN);
    IG_fd = GPIO::GPIOConst::getInstance()->getGpioByKey(IG_PIN);
    gp->setDirection(APC_fd, GPIO::OUTPUT);
    gp->setDirection(VVO_fd, GPIO::OUTPUT);
    gp->setDirection(VVF_fd, GPIO::OUTPUT);
    gp->setDirection(OPV_fd, GPIO::OUTPUT);
    gp->setDirection(FPV_fd, GPIO::OUTPUT);
    gp->setDirection(PPV_fd, GPIO::OUTPUT);
    gp->setDirection(IV1_fd, GPIO::OUTPUT);
    gp->setDirection(IV2_fd, GPIO::OUTPUT);
    gp->setDirection(MOV_fd, GPIO::OUTPUT); //TODO swap to uart later
    gp->setDirection(MFV_fd, GPIO::OUTPUT);
    gp->setDirection(IG_fd, GPIO::OUTPUT);

    //TODO set i2c reg
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
    valve_safe_state(); 

    while(1){ // TODO add end state or break
        state.state_mutex.lock();
        read_hardware();
        check_for_emergency();
        state_machine();
        control();
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

// Determine if the a transistion can be made for the current state.
// If the tranistion requriment(s) are met, the settings for the next state 
// are set as it swap to the next state.
int CentralManager::
state_machine() {

    switch(state.current_state) {
    // general
        case eStandby:
            valve_safe_state();
            if(state.last_command.empty()) {
                break;
            }
            else if(strncmp(state.last_command.c_str(), "arm", strlen("arm")) == 0) {
                state.current_state = eArmed;
                state.current_state_name = "armed";
                state.save_file_name = "data/startup.csv";
                datafile.open(state.save_file_name, std::ios_base::app);
                datafile << FILE_HEADER;
            }
            break;

        case eArmed:
            if(state.last_command.empty()) {
                break;
            }
            else if(strncmp(state.last_command.c_str(), "chill", strlen("chill")) == 0) {
                state.current_state = ePreChill;
                state.current_state_name = "pre-chill";
                valve_safe_state(); 
                state.MFV = CRACKED;
            }
            break;

        case ePreChill:
            if(state.last_command.empty()) {
                break;
            }
            else if(strncmp(state.last_command.c_str(), "ready", strlen("ready")) == 0) {
                state.current_state = eReady;
                state.current_state_name = "ready";
                valve_safe_state(); 
            }
            break;

        case eReady:
            if(state.last_command.empty()) {
                break;
            }
            else if(strncmp(state.last_command.c_str(), "pressurize", strlen("pressurize")) == 0) {
                state.current_state = ePressurized;
                state.current_state_name = "pressurized";
                valve_safe_state(); 
                state.VVO = CLOSED;
                state.VVF = CLOSED;
                state.OPV = OPEN;
                state.FPV = OPEN;
            }
            break;

        case ePressurized:
            if(state.last_command.empty()) {
                break;
            }
            else if(strncmp(state.last_command.c_str(), "fire", strlen("fire")) == 0) {
                firetime = parse_fire_command(state.last_command);
                if(firetime > 1) {
                    ++state.fire_count;

                    // make new file for data
                    if(datafile.is_open()){
                        datafile.close();
                    }
                    state.save_file_name = new_file_name();
                    datafile.open(state.save_file_name);
                    datafile << FILE_HEADER;

                    state.current_state = eIgnitionStart;
                    state.current_state_name = "ignition start";
                    state.APC = ON;
                    state.VVO = CLOSED;
                    state.VVF = CLOSED;
                    state.OPV = OPEN;
                    state.FPV = OPEN;
                    state.PPV = CLOSED;
                    state.IV1 = OPEN;
                    state.IV2 = OPEN;
                    state.MFV = CLOSED;
                    state.MOV = CLOSED;
                    state.IG = ON;
                    wait_until_time = std::chrono::system_clock::now() + std::chrono::milliseconds(IGNITION_START_TIME);
                }
            }
            else if(strncmp(state.last_command.c_str(), "shutdown", strlen("shutdown")) == 0) {
                state.current_state = eEmergencyPurge;
                state.current_state_name = "emergency purge";
                bool temp = state.APC; 
		valve_safe_state();    //TODO wait for more info
                state.APC = temp;
                state.VVO = CLOSED;
                state.VVF = CLOSED;
		state.PPV = OPEN;
                wait_until_time = std::chrono::system_clock::now() + std::chrono::seconds(PURGE_TIME);
            }
            break;
    // emergency-stop
        case eEmergencyPurge:
	    if(std::chrono::system_clock::now() >= wait_until_time) {
                state.current_state = eEmergencySafe;
                state.current_state_name = "emergency safe";
                bool temp = state.APC;
                valve_safe_state();
                state.APC = temp;
	        wait_until_time = std::chrono::system_clock::now() + std::chrono::seconds(EMERGENCY_SAFE_TIME);
	    }
            break;

	case eEmergencySafe:
	    if(std::chrono::system_clock::now() >= wait_until_time) {
	        state.current_state = eLockout;
	        state.current_state_name = "lockout";
                state.APC = OFF;
	    }
	    break;

        case eLockout:
            if(strncmp(state.last_command.c_str(), "shutdown", strlen("shutdown")) == 0) {
                state.current_state = eSafeShutdown;
                state.current_state_name = "safe shutdown";
	    }
            break;

        case eSafeShutdown: // dead state
            if(datafile.is_open())
                datafile.close();
            // TODO end this thread
            break;

    // firing sequence
        case eIgnitionStart:
            if(std::chrono::system_clock::now() >= wait_until_time) {
                state.current_state = eIgnitionOxidize;
                state.current_state_name = "ignition oxidize";
                state.APC = ON;
                state.VVO = CLOSED;
                state.VVF = CLOSED;
                state.OPV = OPEN;
                state.FPV = OPEN;
                state.PPV = CLOSED;
                state.IV1 = OPEN;
                state.IV2 = OPEN;
                state.MFV = CLOSED;
                state.MOV = OPEN;
                state.IG = ON;
                wait_until_time = std::chrono::system_clock::now() + std::chrono::milliseconds(IGNITION_OXIDIZE_TIME);
            }
            break;

	case eIgnitionOxidize:
	    if(std::chrono::system_clock::now() >= wait_until_time) {
		state.current_state = eIgnitionMain;
		state.current_state_name = "ignition main";
                state.APC = ON;
		state.VVO = CLOSED;
		state.VVF = CLOSED;
		state.OPV = OPEN;
		state.FPV = OPEN;
		state.PPV = CLOSED;
		state.IV1 = OPEN;
		state.IV2 = OPEN;
		state.MFV = OPEN;
		state.MOV = OPEN;
		state.IG = ON;
		wait_until_time = std::chrono::system_clock::now() + std::chrono::milliseconds(IGNITION_MAIN_TIME);
	    }
	    break;



        case eIgnitionMain:
            if(std::chrono::system_clock::now() >= wait_until_time) {
                state.current_state = eFiring;
                state.current_state_name = "firing";
                state.APC = ON;
                state.VVO = CLOSED;
                state.VVF = CLOSED;
                state.OPV = OPEN;
                state.FPV = OPEN;
                state.PPV = CLOSED;
                state.IV1 = CLOSED;
                state.IV2 = CLOSED;
                state.MFV = OPEN;
                state.MOV = OPEN;
                state.IG = OFF;
                wait_until_time = std::chrono::system_clock::now() + std::chrono::seconds(firetime-1);
            }
            break;

        case eFiring:
            if(std::chrono::system_clock::now() >= wait_until_time) {
                state.current_state = eFiringStop;
                state.current_state_name = "firing stop";
                state.APC = ON;
                state.VVO = CLOSED;
                state.VVF = CLOSED;
                state.OPV = OPEN;
                state.FPV = OPEN;
                state.PPV = CLOSED;
                state.IV1 = CLOSED;
                state.IV2 = CLOSED;
                state.MFV = CLOSED;
                state.MOV = CLOSED;
                state.IG = OFF;
                wait_until_time = std::chrono::system_clock::now() + std::chrono::milliseconds(FIRING_STOP_TIME);
            }
            break;

        case eFiringStop:
            if(std::chrono::system_clock::now() >= wait_until_time) {
                state.current_state = ePurge;
                state.current_state_name = "purge";
                state.APC = ON;
                state.VVO = CLOSED;
                state.VVF = CLOSED;
                state.OPV = CLOSED;
                state.FPV = CLOSED;
                state.PPV = OPEN;
                state.IV1 = CLOSED;
                state.IV2 = CLOSED;
                state.MFV = CLOSED;
                state.MOV = CLOSED;
                state.IG = OFF;
                wait_until_time = std::chrono::system_clock::now() + std::chrono::seconds(PURGE_TIME);
            }
            break;

        case ePurge:
            if(std::chrono::system_clock::now() >= wait_until_time) {
                state.current_state = ePressurized;
                state.current_state_name = "pressurized";
                valve_safe_state(); 
                state.VVO = CLOSED;
                state.VVF = CLOSED:
                state.OPV = OPEN;
                state.FPV = OPEN;
            }
            break;

        default: // should never happen
            state.current_state = eEmergencyPurge;
            state.current_state_name = "emergency purge";
            break;
    }

    state.last_command = ""; // clear buffer
    return 1;
}

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
    control_valve(state.APC, APC_fd); //is this correct or should it be in the state.IG formatlike below 
    control_valve(state.VVO, VVO_fd);
    control_valve(state.VVF, VVF_fd);
    control_valve(state.OPV, OPV_fd);
    control_valve(state.FPV, FPV_fd);
    control_valve(state.PPV, PPV_fd);
    control_valve(state.IV1, IV1_fd);
    control_valve(state.IV2, IV2_fd);
    control_valve(state.MFV, MFV_fd);
    control_valve(state.MOV, MOV_fd); // TODO UART
    if(state.IG ==  ON)
        gp->setValue(IG_fd, GPIO::HIGH);
    else if(state.IG == OFF)
        gp->setValue(IG_fd, GPIO::LOW);

    return 1;
}

// Handle reading all the I2C sensors
void CentralManager::
valve_safe_state() {
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

