#include "tsar_lowrate_daq.h"

//
// TSAR Low-Rate Data Acquisition Implementation
//
lowrate_daq::lowrate_daq() : device(NULL), addr_1(0), addr_2(0), addr_3(0), addr_4(0){}

lowrate_daq::lowrate_daq(const char* device, const int addr_1, const int addr_2, const int addr_3, const int addr_4){
	this->device = new char[strlen(device) + 1];
	strcpy(this->device, device);
	strcat(this->device, "\0");
}

lowrate_daq::~lowrate_daq(){
	if(device) delete device;
	device = NULL;
}

void lowrate_daq::read_adc(const int *&){
	
}
