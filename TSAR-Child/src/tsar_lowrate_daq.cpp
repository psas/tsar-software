#include "tsar_lowrate_daq.h"

//
// TSAR Low-Rate Data Acquisition Implementation
//
lowrate_daq::lowrate_daq() : device(NULL), addr_1(0), addr_2(0), addr_3(0), addr_4(0){}

lowrate_daq::lowrate_daq(const char* device, const int addr_1, const int addr_2, const int addr_3, const int addr_4){
	this->device = new char[strlen(device) + 1];
	strcpy(this->device, device);
	strcat(this->device, "\0");

	chips = new mcp342x[4];
	chips[0] = mcp342x(addr_1, this->device);
	chips[1] = mcp342x(addr_2, this->device);
	chips[2] = mcp342x(addr_3, this->device);
	chips[3] = mcp342x(addr_4, this->device);

	for(int i = 0; i < 4; ++i)
		chips[i].set_settings(true, 260, 1);
}

lowrate_daq::~lowrate_daq(){
	if(device) delete device;
	device = NULL;
}

void lowrate_daq::read_adcs(const int *& data){
	data = new int[16];
	for(int i = 0; i < 4; ++i){
		for(int j = 0; j < 4; ++j){
			chips[i].set_channel(j);
			chips[i].start_read();
			while(!chips[i].is_ready()) usleep(S);
			data[i * 4 + j] = chips[i].read_register();
		}
	}
}
