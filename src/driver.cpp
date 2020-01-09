#include <iostream>
#include "state.h"

using namespace std;

int main(int argc, char** argv){
	string input = "";
	State tstate;

	while(input != "stop"){
		cout << "> ";
		cin >> input;
		if(!tstate.machine(input)) cerr << "Invalid state or sequence!" << endl << endl;
		cout << tstate << endl;
	}
}
