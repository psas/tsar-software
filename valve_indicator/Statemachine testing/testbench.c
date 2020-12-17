//testbench for state machine

#include "StateMachine.h"

int main()
{
	int batteryState = 1;
	int valveCState[9];
	int valveExState[9];
	
	int i = 0;
	for (i=0; i<9; ++i)
	{
		valveCState[i] = 0;
	}
	
	i = 0;
	for (i=0; i<9; ++i)
	{
		valveExState[i] = 0;
	}
	
	int checkIt = mainCheck(valveCState,valveExState,batteryState);
	
	printf("We got %d from mainCheck\n", checkIt);
	
	return 0;
}