#ifndef TSAR_LOWRATE_DAQ
#define TSAR_LOWRATE_DAQ

#include <iostream>
#include <string.h>
#include "mcp342x.h"

class lowrate_daq{
	private:
		int addr_1, addr_2, addr_3, addr_4;
		char* device;
		mcp342x* chips;

	public:
	lowrate_daq();
	lowrate_daq(const char*, const int, const int, const int, const int);
	~lowrate_daq();

	void read_adc(const int*&);
};

#endif
