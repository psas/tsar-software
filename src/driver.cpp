#include <iostream>
#include "state.h"

using namespace std;

int main(int argc, char** argv){
	string input = "";
	State tstate;

	while(input != "stop"){
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
	}
}
