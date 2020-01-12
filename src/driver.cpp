#include <iostream>
#include "state.h"

using namespace std;

#define CMDLINE true

void test_happy_path(State& s){
    for(int t = SS0; t <= PURGE; ++t){
        s.machine((enum state_type) t);
        cout << s << endl;
    }
}

int main(int argc, char** argv){
    State hp_test;
    test_happy_path(hp_test);

	string input;
	State tstate;

	while(CMDLINE && input != "stop"){
		cout << "> ";
		cin >> input;
        try {
            tstate.machine(input);
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
