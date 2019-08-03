#include <bitset>
#include <string.h>
#include <ncurses.h>
#include "tsar_lowrate_daq.h"
#include "mcp342x.h"

#define S 1

using namespace std;

// int main(const int argc, const char** argv) {
//     // CommandControl CC;
//     // CC.start_system();
//
// 	int row, col;
//
//     mcp342x adc(0x68, "/dev/i2c-1");
// 	adc.set_settings(true, 260, 1);
//     cout << "Created an ADC chip: " << adc << endl;
//
// 	initscr();
// 	getmaxyx(stdscr, row, col);
//
//     while(true){
// 		for(int i = 1; i <= 4; ++i){
// 				int delay = 0;
// 				mvprintw(0 + 3 * i, 0, "Setting channel: %i\n", i);
// 				adc.set_channel(i);
// 				adc.start_read();
// 				while(!adc.is_ready()){
// 					usleep(S);
// 					delay += S;
// 				}
// 				mvprintw(1 + 3 * i, 0, "Waited for %i micro-seconds (%i ms) to read!", delay, (delay / 1000));
// 				int response = adc.read_register();
// 				mvprintw(2 + 3 * i, 0, "Got response from chip: %s (%i)\t\t", bitset<32>(response).to_string().c_str(), response);
// 				usleep(S);
// 		}
// 		refresh();
//   	}
//
// 	endwin();
//
//     return 0;
// }

//
// Continuity test
//
int main(const int argc, const char** argv){

	//lowrate_daq board("/dev/i2c-1", 0x68, 0, 0, 0);

	mcp342x adc(0x68, "/dev/i2c-1");
	adc.set_settings(true, 260, 1);
	adc.set_channel(1);
	cout << "Created an ADC chip: " << adc << endl;

	while(true){
		adc.start_read();
		while(!adc.is_ready())
			usleep(S);

		int data = adc.read_register();
		cout << time(NULL) << "," << data << "," << bitset<32>(data).to_string() << endl;
	}

	return 0;
}
