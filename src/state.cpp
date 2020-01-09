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

State::State(){
	curr_state = SS0;
	prev_state = SS0;

	curr_state_name = state_names[curr_state];
	prev_state_name = state_names[prev_state];
	user_input = "N/A";
	estop = false;

	data_file = "dump.log";

	lock.lock();
	safe_state_zero();
	lock.unlock();
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
	this->IG1 = IG1;
}

void State::safe_state_zero(){
	SOV1 = CLOSED;
	SOV2 = CLOSED;
	SOV3 = CLOSED;
	SOV4 = OPEN;
	SOV5 = CLOSED;
	SOV6 = CLOSED;
	SOV7 = CLOSED;
	SOV8 = OPEN;

	IG1 = CLOSED;
	IG2 = CLOSED;
}

bool State::machine(const std::string input) {
	lock.lock();
	user_input = input;
	bool valid_ss = true;

	if(user_input == "standby") {
		if(curr_state == SS0) {	// Assert correct prereqisite state
			prev_state = curr_state;							 // Record prev state
			curr_state = SS0;								 // Change state
			set(CLOSED, CLOSED, CLOSED, OPEN, CLOSED, CLOSED, CLOSED, OPEN, CLOSED, CLOSED); // Change AC values
		}
		else valid_ss = false;
	}
	else if(user_input == "pressurize") {
		if(curr_state == SS0) {	// Assert correct prereqisite state
			prev_state = curr_state;							 // Record prev state
			curr_state = PRESSURIZE;							 // Change state
			set(CLOSED, CLOSED, CLOSED, OPEN, CLOSED, CLOSED, CLOSED, OPEN, CLOSED, CLOSED); // Change AC values
		}
		else valid_ss = false;
	}
	else valid_ss = false;

	curr_state_name = state_names[curr_state]; // Update curr name
	prev_state_name = state_names[prev_state]; // Update prev name

	lock.unlock();
	return valid_ss;
}

std::ostream& operator<< (std::ostream& buffer, const State& src){
	buffer << "current: " << src.curr_state_name << std::endl
	       << "previous: " << src.prev_state_name << std::endl
	       << "input: " << src.user_input << std::endl
	       << "e-stop: " << bool_to_str(src.estop) << std::endl
	       << "data-file: " << src.data_file << std::endl
	       << "SOV's: [ ";

	buffer << src.SOV1 << " "
	       << src.SOV2 << " "
	       << src.SOV3 << " "
	       << src.SOV4 << " "
	       << src.SOV5 << " "
	       << src.SOV6 << " "
	       << src.SOV7 << " "
	       << src.SOV8 << " ";

	buffer << "] }";

	return buffer;
}

std::string bool_to_str(bool x){ return x ? "true" : "false"; }
