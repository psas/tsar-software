#include <iostream>
#include "state.h"

using namespace std;

int main(int argc, char** argv){
	string input = "";
	State tstate;

    cout << tstate << endl;

    /*tstate.machine("standby");
    cout << tstate << endl;
    tstate.machine("pressurize");
    cout << tstate << endl;
    tstate.machine("ignite");
    cout << tstate << endl;
    tstate.machine("oxidize");
    cout << tstate << endl;
    tstate.machine("fuel");
    cout << tstate << endl;
    tstate.machine("fire");
    cout << tstate << endl;
    tstate.machine("purge");
    cout << tstate << endl;*/

	/*while(input != "stop"){
		cout << "> ";
		cin >> input;
        try {
            tstate.machine(input);
		    cout << tstate << endl;
        }
        catch(BAD_PREREQ e) {
            cerr << "Invalid sequence: " << endl
                 << "\t" << e.message << endl;
        }
	}*/
}
