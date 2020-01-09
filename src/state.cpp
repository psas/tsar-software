#include "state.h"

const char* state_names[] = {
"safe-state-0",
"pressurized",
"ignition",
"oxidizer-start",
"fuel-start",
"firing",
"purging"
};

const char* state_modes[] = {
"CLOSED",
"OPENED",
"CRACKED"
};

// Error spec implementation
BAD_PREREQ::BAD_PREREQ(std::string message) : message(message) {}

// State Object implementation
State::State(){
	curr_state = SS0;
	prev_state = SS0;

	curr_state_name = state_names[curr_state];
	prev_state_name = "N/A";
	user_input = "N/A";
	estop = false;

	data_file = "dump.log";

	lock.lock();
	safe_state_zero();
	lock.unlock();
}

void State::assert_state(state_type expected) {
    if(curr_state != expected){
        lock.unlock();
        std::string message("cannot move from: ");
        message += state_names[expected];
        message += " to: ";
        message += user_input;
        message += " while in: ";
        message += state_names[curr_state];
        throw BAD_PREREQ(message);
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

void State::safe_state_zero(){
    set(CLOSED, CLOSED, CLOSED, OPEN, CLOSED, CLOSED, CLOSED, OPEN, CLOSED, CLOSED);
}

/*
    The general algorithm for changing state looks like this:
        1.) Acquire the state lock
        2.) Assert that the correct prerequisite state is the current state
        3.) Log the current state as the new previous state
        4.) Set the new state properties (Valves, Igniters, etc.)
        5.) Update the state + state name strings
        6.) Release the state lock
*/
void State::machine(const std::string input) {
	lock.lock();
	user_input = input;

	if(user_input == "standby") {
        assert_state(SS0);
		prev_state = curr_state;
        //safe_state_zero();
        set(CLOSED, CLOSED, CLOSED, OPEN, CLOSED, CLOSED, CLOSED, OPEN, CLOSED, CLOSED);
		curr_state = SS0;
	}
    else if(user_input == "pressurize") {
        assert_state(SS0);
		prev_state = curr_state;
        set(OPEN, OPEN, CLOSED, CLOSED, CLOSED, CLOSED, CLOSED, CLOSED, CLOSED, CLOSED);
		curr_state = PRESSURIZE;
	}
    else if(user_input == "ignite") {
        assert_state(PRESSURIZE);
		prev_state = curr_state;
        set(OPEN, OPEN, CLOSED, CLOSED, CLOSED, CLOSED, CLOSED, CLOSED, OPEN, OPEN);
		curr_state = IGNITE;
	}
    else if(user_input == "oxidize") {
        assert_state(IGNITE);
		prev_state = curr_state;
        set(OPEN, OPEN, CLOSED, CLOSED, CLOSED, OPEN, CLOSED, CLOSED, OPEN, OPEN);
		curr_state = O_START;
	}
    else if(user_input == "fuel") {
        assert_state(O_START);
		prev_state = curr_state;
        set(OPEN, OPEN, CLOSED, CLOSED, OPEN, OPEN, CLOSED, CLOSED, OPEN, OPEN);
		curr_state = F_START;
	}
    else if(user_input == "fire") {
        assert_state(F_START);
		prev_state = curr_state;
        set(OPEN, OPEN, CLOSED, CLOSED, OPEN, OPEN, CLOSED, CLOSED, CLOSED, CLOSED);
		curr_state = FIRE;
	}
    else if(user_input == "purge") {
        assert_state(FIRE);
		prev_state = curr_state;
        set(CLOSED, CLOSED, OPEN, CLOSED, CLOSED, CLOSED, CLOSED, CLOSED, CLOSED, CLOSED);
		curr_state = PURGE;
	}

	curr_state_name = state_names[curr_state]; // Update curr name
	prev_state_name = state_names[prev_state]; // Update prev name
	lock.unlock();
}

std::ostream& operator<< (std::ostream& buffer, const State& src){
	buffer << "current:  " << src.curr_state_name << std::endl
	       << "previous: " << src.prev_state_name << std::endl << std::endl
	       /*<< "input: " << src.user_input << std::endl
	       << "e-stop: " << bool_to_str(src.estop) << std::endl
	       << "data-file: " << src.data_file << std::endl*/
           << "Properties:" << std::endl
           << "\tPurge N2: " << state_modes[src.SOV3] << std::endl << std::endl

           << "\tFuel Pressure: " << state_modes[src.SOV1] << std::endl
           << "\tLOX Pressure:  " << state_modes[src.SOV2] << std::endl << std::endl

           << "\tSolenoid Vent Valve 1: " << state_modes[src.SOV4] << std::endl
           << "\tSolenoid Vent Valve 2: " << state_modes[src.SOV8] << std::endl << std::endl

           << "\tFuel Main: " << state_modes[src.SOV5] << std::endl
           << "\tLOX Main:  " << state_modes[src.SOV6] << std::endl
           << "\tLOX Fill:  " << state_modes[src.SOV7] << std::endl << std::endl

           << "\tIgniter 1: " << state_modes[src.IG1] << std::endl
           << "\tIgniter 2: " << state_modes[src.IG2] << std::endl;

	return buffer;
}

std::string bool_to_str(bool x){ return x ? "true" : "false"; }
