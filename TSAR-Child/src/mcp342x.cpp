#include "mcp342x.h"

//
// MCP342X Implementation
//
mcp342x::mcp342x() : address(0), file_descriptor(0), device(NULL){}
mcp342x::mcp342x(const int address, const char* input_device) : address(address){
  device = new char[strlen(input_device) + 1];
  strcpy(device, input_device);
  strcat(device, "\0");
  configuration = CFG_OC;
 
  file_descriptor = open(device, O_RDWR);

  if(file_descriptor < 0){
    std::cerr << "Device " << device << " couldn't be opened!" << std::endl << "Gracefully exiting" << std::endl;
    exit(0);
  }

  ioctl(file_descriptor, I2C_SLAVE, address);
}

mcp342x::~mcp342x(){
  std::cout << "Closing the pipe: " << file_descriptor << std::endl;
  close(file_descriptor);

  if(device) delete device;
  device = NULL;
}

const int mcp342x::to_signed(const int data){
	return data ^ 1;		
}

const int mcp342x::set_settings(const bool one_shot, const int sample_rate, const int pga_gain){
	configuration &= ~(CFG_OC | (CFG_S1 | CFG_S0) | (CFG_G1 | CFG_G0));

	// Configure One Shot
	if(one_shot) configuration |= CFG_OC;

	// Configure Sample Rate
	switch(sample_rate){
		case 240:
			configuration |= CFG_240SPS;
			break;
		case 60:
			configuration |= CFG_60SPS;
			break;
		case 15:
			configuration |= CFG_15SPS;
			break;
		case 3:
		default:
			configuration |= CFG_3SPS;
			break;
	}

	// Configure PGA Gain
	switch(pga_gain){
		case 1:
			configuration |= CFG_X1;
			break;
		case 2:
			configuration |= CFG_X2;
			break;
		case 4:
			configuration |= CFG_X4;
			break;
		case 8:
		default:
			configuration |= CFG_X8;
			break;
	}

	std::cout << "Set settings: " << std::bitset<8>(configuration).to_string() << std::endl;

	return write(file_descriptor, &configuration, 1);
}

const int mcp342x::set_channel(const int channel){
	configuration &= ~(CFG_C1 | CFG_C0); // Wipes the current channel config
	
	switch(channel){
		case 1: 
			configuration |= CFG_CHAN1;
			break;
		case 2:
			configuration |= CFG_CHAN2;
			break;
		case 3:
			configuration |= CFG_CHAN3;
			break;
		case 4:
			configuration |= CFG_CHAN4;
			break;
	}

	std::cout << "Set channel: " << std::bitset<8>(configuration).to_string() << std::endl;

	return write(file_descriptor, &configuration, 1);
}

void mcp342x::start_read(){
	__u8 r_configuration = (configuration | CFG_RDY);
	write(file_descriptor, &r_configuration, 1);
}

const bool mcp342x::is_ready(){
	__u8 data[5];
	bool is_18 = (configuration & CFG_3SPS) != 0;
	
	read(file_descriptor, data, is_18 ? 4 : 3);
	return (data[is_18 ? 3 : 2] & CFG_RDY) == 0;
}

const int mcp342x::read_register(){
	__u8 data[5];
	bool is_18 = (configuration & CFG_3SPS) != 0;
	
	read(file_descriptor, data, is_18 ? 3 : 2);

	if(is_18)
		return (((int) data[0]) << 16) + (((int) data[1]) << 8) + ((int) data[2]);
	else
		return (((int) data[0]) << 8) + ((int) data[1]);
}

std::ostream& operator <<(std::ostream& buffer, const mcp342x& src){
	buffer << "{ addr: " << src.address << ", fd: " << src.file_descriptor << ", dev: " << src.device << ", conf: " << src.configuration<< " }";
	return buffer;
}
