#include "state.h"

const char* cmd_names[] = {
"prefill",
"fill",
"standby",
"pressurize",
"ignite",
"oxidize",
"fuel",
"fire",
"purge",
"blowdown"
};

const char* state_names[] = {
"lox-prefill",
"lox-fill",
"safe-state-0",
"pressurized",
"ignition",
"oxidizer-start",
"fuel-start",
"firing",
"purging",
"lox-blowdown"
};

const char* state_modes[] = {
"CLOSED",
"OPENED",
"CRACKED"
};

// Error spec implementation
BAD_PREREQ::BAD_PREREQ(state_type curr, state_type from, state_type to)
    : curr(curr), from(from), to(to) {
        message = "Invalid sequence\n";
        message += "\tCannot jump from State<";
        message += state_names[from];
        message += "> to State<";
        message += state_names[to];
        message += ">, while only in State<";
        message += state_names[curr];
        message += ">\n";
}

BAD_CMD::BAD_CMD(std::string input){
    message = "Command: " + input + " was not recognized!\n\nAvailable Commands:";
    for(int i = 0; i < 10; ++i){
        message += "\n\t";
        message += std::to_string(i + 1);
        message += ".) ";
        message += cmd_names[i];
    }
    message += "\n";
}

UNLOCKED_ASSERTION::UNLOCKED_ASSERTION()
    : message("fatal: Lock was not obtained before assertion!") {}

// State Object implementation
State::State(){
	curr_state = (enum state_type) 0;
	prev_state = (enum state_type) 0;

	curr_state_name = state_names[curr_state];
	prev_state_name = "N/A";
	user_input = "N/A";
	estop = false;

	data_file = "dump.log";

	lock.lock();
	safe_state_zero();
	lock.unlock();
}

/*
    An internal tool to assert the correct state
    This function will throw errors instead of returning
        if the state is invalid or if the lock is not held.
*/
void State::assert_state(state_type expected, state_type transition_to) {
    if(curr_state != expected){
        if(lock.try_lock()){
            lock.unlock();
            throw UNLOCKED_ASSERTION();
        }
        lock.unlock();
        throw BAD_PREREQ(curr_state, expected, transition_to);
    }
}

void State::set(bool SOV1, bool SOV2, bool SOV3, bool SOV4, bool SOV5, bool SOV6, bool SOV7, bool SOV8, bool IG1, bool IG2) {
	this->SOV1 = SOV1;
	this->SOV2 = SOV2;
	this->SOV3 = SOV3;
	this->SOV4 = SOV4;
	this->SOV5 = SOV5;
	this->SOV6 = SOV6;
	this->SOV7 = SOV7;
	this->SOV8 = SOV8;

	this->IG1 = IG1;
	this->IG2 = IG2;
}

void State::safe_state_zero() {
    set(CLOSED, CLOSED, CLOSED, OPEN, CLOSED, CLOSED, CLOSED, OPEN, CLOSED, CLOSED);
}

void State::machine(const std::string input) {
    user_input = tolower(input);

    if(user_input == "prefill") machine(LOX_PREFILL);
    else if(user_input == "fill") machine(LOX_FILL);
    else if(user_input == "standby") machine(SS0);
    else if(user_input == "pressurize") machine(PRESSURIZE);
    else if(user_input == "ignite") machine(IGNITE);
    else if(user_input == "oxidize") machine(O_START);
    else if(user_input == "fuel") machine(F_START);
    else if(user_input == "fire") machine(FIRE);
    else if(user_input == "purge") machine(PURGE);
    else if(user_input == "stop" || user_input == "status") {} // No op
    else throw BAD_CMD(input);
}

/*
    The general algorithm for changing state looks like this:
        1.) Acquire the state lock
        2.) Assert that the correct prerequisite state is the current state
        3.) Set the new state properties (Valves, Igniters, etc.)
        4.) Log the current state as the new previous state
        5.) Update the state + state name strings
        6.) Release the state lock
*/
void State::machine(const state_type expected) {
	lock.lock();

    if(expected == LOX_PREFILL) {
        assert_state(LOX_PREFILL, expected);
        set(CLOSED, OPEN, CLOSED, CLOSED, CLOSED, OPEN, CLOSED, CLOSED, CLOSED, CLOSED);
	}
    else if(expected == LOX_FILL) {
        assert_state(LOX_PREFILL, expected);
        set(CLOSED, CLOSED, CLOSED, CLOSED, CLOSED, OPEN, OPEN, OPEN, CLOSED, CLOSED);
	}
    else if(expected == SS0) {
        assert_state(LOX_FILL, expected);
        safe_state_zero();
	}
    else if(expected == PRESSURIZE) {
        assert_state(SS0, expected);
        set(OPEN, OPEN, CLOSED, CLOSED, CLOSED, CLOSED, CLOSED, CLOSED, CLOSED, CLOSED);
	}
    else if(expected == IGNITE) {
        assert_state(PRESSURIZE, expected);
        set(OPEN, OPEN, CLOSED, CLOSED, CLOSED, CLOSED, CLOSED, CLOSED, OPEN, OPEN);
	}
    else if(expected == O_START) {
        assert_state(IGNITE, expected);
        set(OPEN, OPEN, CLOSED, CLOSED, CLOSED, OPEN, CLOSED, CLOSED, OPEN, OPEN);
	}
    else if(expected == F_START) {
        assert_state(O_START, expected);
        set(OPEN, OPEN, CLOSED, CLOSED, OPEN, OPEN, CLOSED, CLOSED, OPEN, OPEN);
	}
    else if(expected == FIRE) {
        assert_state(F_START, expected);
        set(OPEN, OPEN, CLOSED, CLOSED, OPEN, OPEN, CLOSED, CLOSED, CLOSED, CLOSED);
	}
    else if(expected == PURGE) {
        assert_state(FIRE, expected);
        set(CLOSED, CLOSED, OPEN, CLOSED, CLOSED, CLOSED, CLOSED, CLOSED, CLOSED, CLOSED);
	}
    else if(expected == LOX_BLOWDOWN) {
        assert_state(PURGE, expected);
        set(CLOSED, OPEN, CLOSED, CLOSED, CLOSED, OPEN, CLOSED, CLOSED, CLOSED, CLOSED);
	}

	prev_state = curr_state;
	curr_state = expected;
	curr_state_name = state_names[curr_state]; // Update curr name
	prev_state_name = state_names[prev_state]; // Update prev name
	lock.unlock();
}

void State::toggle(const std::string input) {
    user_input = tolower(input);

    if(user_input == "sov1") toggle(SOV1);
    else if(user_input == "sov2") toggle(SOV2);
    else if(user_input == "sov3") toggle(SOV3);
    else if(user_input == "sov4") toggle(SOV4);
    else if(user_input == "sov5") toggle(SOV5);
    else if(user_input == "sov6") toggle(SOV6);
    else if(user_input == "sov7") toggle(SOV7);
    else if(user_input == "sov8") toggle(SOV8);
    else if(user_input == "ig1") toggle(IG1);
    else if(user_input == "ig2") toggle(IG2);
    else if(user_input == "stop" || user_input == "status") {} // No op
    else throw BAD_CMD(input);
}

void State::toggle(bool& valve) { valve = (valve ? false : true); }

std::ostream& operator<< (std::ostream& buffer, const State& src){
	buffer << "current:  " << src.curr_state_name << std::endl
	       << "previous: " << src.prev_state_name << std::endl << std::endl

           << "Properties:" << std::endl
           << "\t[SOV3] Purge N2: " << state_modes[src.SOV3] << std::endl << std::endl

           << "\t[SOV1] Fuel Pressure: " << state_modes[src.SOV1] << std::endl
           << "\t[SOV2] LOX Pressure:  " << state_modes[src.SOV2] << std::endl << std::endl

           << "\t[SOV4] Solenoid Vent Valve 1: " << state_modes[src.SOV4] << std::endl
           << "\t[SOV8] Solenoid Vent Valve 2: " << state_modes[src.SOV8] << std::endl << std::endl

           << "\t[SOV5] Fuel Main: " << state_modes[src.SOV5] << std::endl
           << "\t[SOV6] LOX Main:  " << state_modes[src.SOV6] << std::endl
           << "\t[SOV7] LOX Fill:  " << state_modes[src.SOV7] << std::endl << std::endl

           << "\t[IG1] Igniter 1: " << state_modes[src.IG1] << std::endl
           << "\t[IG2] Igniter 2: " << state_modes[src.IG2] << std::endl;

	return buffer;
}

std::string bool_to_str(bool x){ return x ? "true" : "false"; }

std::string tolower(const std::string input){
    char* res = new char[input.length() + 1];
    for(int i = 0; i < (int) input.length(); ++i) res[i] = tolower(input[i]);
    return std::string(res); 
}
