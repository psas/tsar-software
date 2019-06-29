#include "command_control.h"
#include "mcp342x.h"

#define S 1000000

using namespace std;

int main(const int argc, const char** argv) {
    // CommandControl CC;
    // CC.start_system();

    mcp342x adc(0x68, "/dev/i2c-1", 10);

    cout << "Entering main loop..." << endl;

    while(true){
  		adc.read();
  		usleep(S);
  	}

    return 0;
}
