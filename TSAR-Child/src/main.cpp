#include "mcp342x.h"

#define S 1000000

using namespace std;

int main(const int argc, const char** argv) {
    // CommandControl CC;
    // CC.start_system();

    mcp342x adc(0x68, "/dev/i2c-1");

    cout << "Created ADC: " << adc << endl << "Entering main loop..." << endl;

    while(true){
		char buffer[3];

  		int response = adc.read_register(3, buffer);

		cout << "Got response from chip: " << response << endl;
  		usleep(S);
  	}

    return 0;
}
