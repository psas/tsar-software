#include <bitset>
#include <string.h>
#include <ncurses.h>
#include "mcp342x.h"

#define S 1

using namespace std;

int main(const int argc, const char** argv) {
    // CommandControl CC;
    // CC.start_system();

	int row, col;

    mcp342x adc(0x68, "/dev/i2c-1");
    cout << "Created ADC: " << adc << endl << "Entering main loop..." << endl;

	adc.set_settings(true, 260, 1);

	//initscr();
	//getmaxyx(stdscr, row, col);

    while(true){
		//for(int i = 1; i <= 4; ++i){
				int i = 1;
				int delay = 0;
				//mvprintw(0 + 3 * i, 0, "Setting channel: %i\n", i);
				adc.set_channel(i);
				adc.start_read();
				while(!adc.is_ready()){
					usleep(S);
					delay += S;
				}
				//mvprintw(1 + 3 * i, 0, "Waited for %i micro-seconds (%i ms) to read!", delay, (delay / 1000));
				int response = adc.read_register();
				//mvprintw(2 + 3 * i, 0, "Got response from chip: %s", bitset<32>(response).to_string().c_str());
				cout << response << ",";
				usleep(S);
		//}
		//refresh();
  	}

	//endwin();

    return 0;
}
