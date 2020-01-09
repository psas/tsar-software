#ifndef TSAR_STATE
#define TSAR_STATE

#include <iostream>
#include <string>
#include <mutex>

#define CLOSED  0
#define OPEN    1
#define CRACKED 2

enum state_type { SS0, PRESSURIZE, IGNITE, O_START, F_START, FIRE, PURGE };

struct BAD_PREREQ {
    BAD_PREREQ(state_type, state_type, state_type);
    state_type curr, from, to;
    std::string message;
};

struct BAD_CMD {
    BAD_CMD(std::string);
    std::string message;
};

class State {
	private: 
		// State management things
		state_type curr_state;
		state_type prev_state;
		std::string curr_state_name; // User friendly current state name
		std::string prev_state_name; // User-friendly previous state name
		std::string user_input;	// Last seen user input (command)
		bool estop; // Emergency stop

        // Special internal functions only
        void assert_state(state_type, state_type);
		void set(bool, bool, bool, bool, bool, bool, bool, bool, bool, bool);

		std::string data_file; // The file to dump DSP/DAQ data to
		std::mutex lock; // State lock

		// State properties (Valves, vents, and servos, oh my)
		bool SOV1; // Fuel Pressure
		bool SOV2; // LOX Pressure
		bool SOV3; // Purge N2
		bool SOV4; // Solenoid Vent Valve 1
		bool SOV5; // Fuel Main
		bool SOV6; // LOX Main
		bool SOV7; // LOX Fill
		bool SOV8; // Solenoid Vent Valve 2

		bool IG1; // Igniter Valve 1
		bool IG2; // Igniter Valve 2

	public:
		// Constructors / Destructors
		State();
		void safe_state_zero();
		void machine(const std::string);
		void machine(const state_type);

		friend std::ostream& operator<< (std::ostream&, const State&);
};

std::string bool_to_str(bool);
std::string tolower(const std::string);

#endif
