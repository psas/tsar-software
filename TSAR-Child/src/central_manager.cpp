#include "central_manager.h"

// constructor
CentralManager::
CentralManager() {
    system_epoch = get_time_us();
    state = std::shared_ptr<State>(new State);
    next_print_time = std::chrono::system_clock::now();


    // default values in state struct
    state->current_state = 0;
    state->current_state_name = "standby";
    state->fire_count =0;

    state->VVO = OPEN;
    state->VVF = OPEN;
    state->OPV = CLOSED;
    state->FPV = CLOSED;
    state->PPV = CLOSED;
    state->IV1 = CLOSED;
    state->IV2 = CLOSED;
    state->MFV = CLOSED;
    state->MOV = CLOSED;
    state->IG = OFF;

    // gpio
    gp = GPIO::GPIOManager::getInstance();
    VVO_pin = GPIO::GPIOConst::getInstance()->getGpioByKey(VVO_PIN);
    VVF_pin = GPIO::GPIOConst::getInstance()->getGpioByKey(VVF_PIN);
    OPV_pin = GPIO::GPIOConst::getInstance()->getGpioByKey(OPV_PIN);
    FPV_pin = GPIO::GPIOConst::getInstance()->getGpioByKey(FPV_PIN);
    PPV_pin = GPIO::GPIOConst::getInstance()->getGpioByKey(PPV_PIN);
    IV1_pin = GPIO::GPIOConst::getInstance()->getGpioByKey(IV1_PIN);
    IV2_pin = GPIO::GPIOConst::getInstance()->getGpioByKey(IV2_PIN);
    MFV_pin = GPIO::GPIOConst::getInstance()->getGpioByKey(MFV_PIN); // TODO swap to uart later
    MOV_pin = GPIO::GPIOConst::getInstance()->getGpioByKey(MOV_PIN);
    IG_pin = GPIO::GPIOConst::getInstance()->getGpioByKey(IG_PIN);
    gp->setDirection(VVO_pin, GPIO::OUTPUT);
    gp->setDirection(VVF_pin, GPIO::OUTPUT);
    gp->setDirection(OPV_pin, GPIO::OUTPUT);
    gp->setDirection(FPV_pin, GPIO::OUTPUT);
    gp->setDirection(PPV_pin, GPIO::OUTPUT);
    gp->setDirection(IV1_pin, GPIO::OUTPUT);
    gp->setDirection(IV2_pin, GPIO::OUTPUT);
    gp->setDirection(MOV_pin, GPIO::OUTPUT); //TODO swap to uart later
    gp->setDirection(FPV_pin, GPIO::OUTPUT);
    gp->setDirection(IG_pin, GPIO::OUTPUT);

    //TODO set i2c reg
}


// deconstructor
CentralManager::
~CentralManager() {
    if(datafile.is_open())
        datafile.close();
}


// main loop for central manager, that call the main functions
void CentralManager::
CM_loop() {

   while(1){ // TODO add end state or break
        state->state_mutex.lock();
        read_hardware();
        update();
        state_machine();
        control();
        if(state->current_state >= eArmed && state->current_state != eSafeShutdown) {
            save();
        }
        state->state_mutex.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(CM_DELAY));
    }
}

// Handle reading all the I2C sensors
int CentralManager::read_hardware() {
    return 0;
}


// TODO Updates the user?
int CentralManager::update() {
    if(std::chrono::system_clock::now() >= next_print_time) {
        std::cout << state->current_state_name << std::endl;
        next_print_time = std::chrono::system_clock::now() + std::chrono::milliseconds(PRINT_DELAY);
    }
    return 1;
}


// Determine if the a transistion can be made for the current state.
// If the tranistion requriment(s) are met, the settings for the next state 
// are set as it swap to the next state.
int CentralManager::
state_machine() {

    // check fore emergency command
    if(strncmp(state->last_command.c_str(), "er", strlen("er")) == 0) {
        state->last_command = "";
        if(state->current_state == eStandby) {
            state->current_state = eLockout;
            state->current_state_name = "lockout";
        }
        else {
            state->current_state = eEmergencyPurge;
            state->current_state_name = "emergency purge";
        }
    }

    switch(state->current_state) {
    // general
        case eStandby:
            if(state->last_command.empty()) {
                break;
            }
            else if(strncmp(state->last_command.c_str(), "arm", strlen("arm")) == 0) {
                state->current_state = eArmed;
                state->current_state_name = "armed";
                datafile.open("startup.csv");
                state->VVO = OPEN;
                state->VVF = OPEN;
                state->OPV = CLOSED;
                state->FPV = CLOSED;
                state->PPV = CLOSED;
                state->IV1 = CLOSED;
                state->IV2 = CLOSED;
                state->MFV = CLOSED;
                state->MOV = CLOSED;
                state->IG = OFF;
            }
            else {
                print_input_error(state->last_command, state->current_state_name);
            }
            state->last_command = "";
            break;

        case eArmed:
            if(state->last_command.empty()) {
                break;
            }
            else if(strncmp(state->last_command.c_str(), "chill", strlen("chill")) == 0) {
                state->current_state = ePreChill;
                state->current_state_name = "pre-chill";
                state->VVO = OPEN;
                state->VVF = OPEN;
                state->OPV = CLOSED;
                state->FPV = CLOSED;
                state->PPV = CLOSED;
                state->IV1 = CLOSED;
                state->IV2 = CLOSED;
                state->MFV = CRACKED;
                state->MOV = CLOSED;
                state->IG = OFF;
            }
            else {
                print_input_error(state->last_command, state->current_state_name);
            }
            state->last_command = "";
            break;

        case ePreChill:
            if(state->last_command.empty()) {
                break;
            }
            else if(strncmp(state->last_command.c_str(), "ready", strlen("ready")) == 0) {
                state->current_state = eReady;
                state->current_state_name = "ready";
                state->VVO = OPEN;
                state->VVF = OPEN;
                state->OPV = CLOSED;
                state->FPV = CLOSED;
                state->PPV = CLOSED;
                state->IV1 = CLOSED;
                state->IV2 = CLOSED;
                state->MFV = CLOSED;
                state->MOV = CLOSED;
                state->IG = OFF;
            }
            else {
                print_input_error(state->last_command, state->current_state_name);
            }
            state->last_command = "";
            break;

        case eReady:
            if(state->last_command.empty()) {
                break;
            }
            else if(strncmp(state->last_command.c_str(), "pressurize", strlen("pressurize")) == 0) {
                state->current_state = ePressurized;
                state->current_state_name = "pressurized";
                state->VVO = OPEN;
                state->VVF = OPEN;
                state->OPV = OPEN;
                state->FPV = OPEN;
                state->PPV = CLOSED;
                state->IV1 = CLOSED;
                state->IV2 = CLOSED;
                state->MFV = CLOSED;
                state->MOV = CLOSED;
                state->IG = OFF;
            }
            else {
                print_input_error(state->last_command, state->current_state_name);
            }
            state->last_command = "";
            break;

        case ePressurized:
            if(state->last_command.empty()) {
                break;
            }
            else if(strncmp(state->last_command.c_str(), "fire", strlen("fire")) == 0) {
                firetime = parse_fire_command(state->last_command);
                if(firetime != -1) {
                    ++state->fire_count;

                    // make new file for data
                    if(datafile.is_open()){
                        datafile.close();
                    }
                    datafile.open(new_file_name());

                    state->current_state = eIgnitionStart;
                    state->current_state_name = "ignition start";
                    state->VVO = CLOSED;
                    state->VVF = CLOSED;
                    state->OPV = OPEN;
                    state->FPV = OPEN;
                    state->PPV = CLOSED;
                    state->IV1 = OPEN;
                    state->IV2 = OPEN;
                    state->MFV = CLOSED;
                    state->MOV = CLOSED;
                    state->IG = ON;
                    wait_until_time = std::chrono::system_clock::now() + std::chrono::milliseconds(IGNITION_START_TIME);
                }
            }
            else if(strncmp(state->last_command.c_str(), "shutdown", strlen("shutdown")) == 0) {
                state->current_state = eEmergencyPurge;
                state->current_state_name = "emergency purge";
            }
            else {
                print_input_error(state->last_command, state->current_state_name);
            }
            state->last_command = "";
            break;
    
    // emergency-stop
        case eEmergencyPurge:
            std::cout << TEXT_RED << "\nIN EMERGENCY STATE!!!!!!!!\n\n" << TEXT_WHITE << std::endl;
            state->current_state = eLockout;
            state->current_state_name = "lockout";
            state->VVO = OPEN;
            state->VVF = OPEN;
            state->OPV = CLOSED;
            state->FPV = CLOSED;
            state->PPV = OPEN;
            state->IV1 = CLOSED;
            state->IV2 = CLOSED;
            state->MFV = CLOSED;
            state->MOV = CLOSED;
            state->IG = OFF;
            break;

        case eLockout:
            if(strncmp(state->last_command.c_str(), "shutdown", strlen("shutdown")) == 0) {
                state->current_state = eSafeShutdown;
                state->current_state_name = "safe shutdown";
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
                state->current_state = eIgnitionMain;
                state->current_state_name = "ignition main";
                state->VVO = CLOSED;
                state->VVF = CLOSED;
                state->OPV = OPEN;
                state->FPV = OPEN;
                state->PPV = CLOSED;
                state->IV1 = OPEN;
                state->IV2 = OPEN;
                state->MFV = OPEN;
                state->MOV = OPEN;
                state->IG = ON;
                wait_until_time = std::chrono::system_clock::now() + std::chrono::milliseconds(IGNITION_MAIN_TIME);
            }
            break;

        case eIgnitionMain:
            if(std::chrono::system_clock::now() >= wait_until_time) {
                state->current_state = eFiring;
                state->current_state_name = "firing";
                state->VVO = CLOSED;
                state->VVF = CLOSED;
                state->OPV = OPEN;
                state->FPV = OPEN;
                state->PPV = CLOSED;
                state->IV1 = CLOSED;
                state->IV2 = CLOSED;
                state->MFV = OPEN;
                state->MOV = OPEN;
                state->IG = OFF;
                wait_until_time = std::chrono::system_clock::now() + std::chrono::seconds(firetime-1);
            }
            break;

        case eFiring:
            if(std::chrono::system_clock::now() >= wait_until_time) {
                state->current_state = eFiringStop;
                state->current_state_name = "firing stop";
                state->VVO = CLOSED;
                state->VVF = CLOSED;
                state->OPV = OPEN;
                state->FPV = OPEN;
                state->PPV = CLOSED;
                state->IV1 = CLOSED;
                state->IV2 = CLOSED;
                state->MFV = CLOSED;
                state->MOV = CLOSED;
                state->IG = OFF;
                wait_until_time = std::chrono::system_clock::now() + std::chrono::milliseconds(FIRING_STOP_TIME);
            }
            break;

        case eFiringStop:
            if(std::chrono::system_clock::now() >= wait_until_time) {
                state->current_state = ePurge;
                state->current_state_name = "purge";
                state->VVO = CLOSED;
                state->VVF = CLOSED;
                state->OPV = CLOSED;
                state->FPV = CLOSED;
                state->PPV = OPEN;
                state->IV1 = CLOSED;
                state->IV2 = CLOSED;
                state->MFV = CLOSED;
                state->MOV = CLOSED;
                state->IG = OFF;
                wait_until_time = std::chrono::system_clock::now() + std::chrono::seconds(PURGE_TIME);
            }
            break;

        case ePurge:
            if(std::chrono::system_clock::now() >= wait_until_time) {
                state->current_state = ePressurized;
                state->current_state_name = "pressurized";
                state->VVO = OPEN;
                state->VVF = OPEN;
                state->OPV = OPEN;
                state->FPV = OPEN;
                state->PPV = CLOSED;
                state->IV1 = CLOSED;
                state->IV2 = CLOSED;
                state->MFV = CLOSED;
                state->MOV = CLOSED;
                state->IG = OFF;
            }
            break;

        default:
            std::cout << TEXT_RED <<"\nUNKNOWN STATE WTF!!!!!!!!\n\n" << TEXT_WHITE << std::endl;
            state->current_state = eEmergencyPurge;
            state->current_state_name = "emergency purge";
            break;
    }

    return 1;
}


// Handles controlling the valve
int CentralManager::
control() { 
    if(state->VVO == OPEN)
        gp->setValue(VVO_pin, GPIO::HIGH);
    else if(state->VVO == CLOSED)
        gp->setValue(VVO_pin, GPIO::LOW);

    if(state->VVF == OPEN)
        gp->setValue(VVF_pin, GPIO::HIGH);
    else if(state->VVF == CLOSED)
        gp->setValue(VVF_pin, GPIO::LOW);

    if(state->FPV == OPEN)
        gp->setValue(FPV_pin, GPIO::HIGH);
    else if(state->FPV == CLOSED)
        gp->setValue(FPV_pin, GPIO::LOW);

    if(state->PPV == OPEN)
        gp->setValue(PPV_pin, GPIO::HIGH);
    else if(state->PPV == CLOSED)
        gp->setValue(PPV_pin, GPIO::LOW);

    if(state->PPV == OPEN)
        gp->setValue(PPV_pin, GPIO::HIGH);
    else if(state->PPV == CLOSED)
        gp->setValue(PPV_pin, GPIO::LOW);

    if(state->IV1 == OPEN)
        gp->setValue(IV1_pin, GPIO::HIGH);
    else if(state->IV1 == CLOSED)
        gp->setValue(IV1_pin, GPIO::LOW);

    if(state->IV2 == OPEN)
        gp->setValue(IV2_pin, GPIO::HIGH);
    else if(state->IV2 == CLOSED)
        gp->setValue(IV2_pin, GPIO::LOW);

    if(state->MFV == OPEN)
        gp->setValue(MFV_pin, GPIO::HIGH);
    else if(state->MFV == CLOSED)
        gp->setValue(MFV_pin, GPIO::LOW);

    if(state->MOV == OPEN)
        gp->setValue(MOV_pin, GPIO::HIGH);
    else if(state->MOV == CLOSED)
        gp->setValue(MOV_pin, GPIO::LOW);

    if(state->IG ==  ON)
        gp->setValue(IG_pin, GPIO::HIGH);
    else if(state->IG == OFF)
        gp->setValue(IG_pin, GPIO::LOW);

    return 1; 
}


// save all the data in the state struct
int CentralManager::
save() {
    if(!datafile.is_open()) {
        std::cout << "No file open\n" << std::endl;
        return 0;
    }

    // TODO add data here
    datafile << (get_time_us() - system_epoch); // TODO put in read hardware
    datafile << ',';
    datafile << state->current_state_name;
    datafile << ',';
    datafile << state->fire_count;
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
            std::cout << firetime  << " is less than 1 secound" << std::endl;
            time_int = -1;
        }
    } catch(...) {
        std::cout << "Failed to parse: " << time  << " as a int" << std::endl;
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
    std::string file = "fire";
    file.append(std::to_string(state->fire_count));
    file.append(".csv");
    return file;
}


// allows the command and control thread to set input the last command
int CentralManager::
input_command(std::string &command){
    state -> state_mutex.lock();
    state -> last_command = command;
    state -> state_mutex.unlock();
    return 0;
}


// prints colored output text for command input errors in the statemachine
void CentralManager::
print_input_error(std::string & command, std::string & state) {
    std::cout << TEXT_YELLOW << "Command: " << command << " not reconizied for " << state << " state\n" << TEXT_WHITE << std::endl;
    return;
}

