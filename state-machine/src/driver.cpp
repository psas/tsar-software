#include <iostream>
#include "state.h"

using namespace std;

#define HP_TEST false

void usage(const std::string name) {
    cout << "Usage: " << name << " [OPTIONS]" << endl
         << "\t-h/--help\tStop it, get some help" << endl
         << "\t-u/--utility-mode\tEnables utility mode allowing for individual vent toggling instead of running the regular test stand sequence" << endl;
}

void test_happy_path(State& s) {
    for(int t = (int) LOX_PREFILL; t <= (int) LOX_BLOWDOWN; ++t){
        s.machine((enum state_type) t);
        cout << s << endl;
    }
}

int main(int argc, char** argv) {
    bool utility_mode = false;

    if(HP_TEST){
        State hp_test;
        test_happy_path(hp_test);
    }

    // Parse arguments
    if(argc > 3){
        usage(argv[0]);
        exit(-1);
    }
    else {
        for(int i = 1; i < argc; ++i){
            std::string arg(argv[i]);
            if(arg == "-h" || arg == "--help"){
                usage(argv[0]);
                exit(0);
            }
            else if(arg == "-u" || arg == "--utility-mode") utility_mode = true;
        }
    }

    // Create the state machine
	string input;
	State tstate;

    // Start the control loop
	while(input != "stop"){
        if(utility_mode) cout << "UTILITY MODE";
		cout << "> ";
		cin >> input;

        try {
            if(utility_mode) tstate.toggle(input);
            else tstate.machine(input);
		    cout << tstate << endl;
        }
        catch(BAD_PREREQ e) { cerr << e.message << endl; }
        catch(BAD_CMD e) { cerr << e.message << endl; }
        catch(UNLOCKED_ASSERTION e) {
            cerr << e.message << endl;
            exit(-1);
        }
        catch(...) {
            cerr << "fatal: Unknown error!" << endl;
            exit(-1);
        }
	}
}
