#include <bitset>
#include "mcp342x.h"

#define S 1

using namespace std;

int main(const int argc, const char** argv) {
    // CommandControl CC;
    // CC.start_system();

    mcp342x adc(0x68, "/dev/i2c-1");
    cout << "Created ADC: " << adc << endl << "Entering main loop..." << endl;

	adc.set_settings(true, 240, 8);

    while(true){
		for(int i = 1; i <= 2; ++i){
				int delay = 0;
				cout << "Reading from: " << adc << endl << "\ton channel: " << 2 << endl;
				adc.set_channel(i);
				adc.start_read();
				while(!adc.is_ready()){
					usleep(S);
					delay += S;
				}
				cout << "Waited for " << delay << " micro-seconds (" << (delay / 1000) << " ms) to read!" << endl;
				int response = adc.read_register();
				cout << "Got response from chip: " << bitset<24>(response).to_string() << endl;
				usleep(S);
		}
  	}

    return 0;
}
