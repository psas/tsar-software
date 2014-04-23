#include "mcp342x.h"

//
// MCP342X Implementation
//
mcp342x::mcp342x() : address(0), file_descriptor(0), device(NULL), register_size(0), register_data(NULL){}
mcp342x::mcp342x(const int address, const char* input_device) :
		address(address), register_size(register_size){
  device = new char[strlen(input_device) + 1];
  strcpy(device, input_device);
  strcat(device, "\0");
 
  file_descriptor = open(device, O_RDWR);

  if(file_descriptor < 0){
    std::cerr << "Device " << device << " couldn't be opened!" << std::endl << "Gracefully exiting" << std::endl;
    exit(0);
  }

  register_data = new char[10];
  ioctl(file_descriptor, I2C_SLAVE, address);
}

mcp342x::~mcp342x(){
  std::cout << "Closing the pipe: " << file_descriptor << std::endl;
  close(file_descriptor);
}

const int mcp342x::read_register(const int size, const char*& data){
	return read(file_descriptor, data, size);
}

std::ostream& operator <<(std::ostream& buffer, const mcp342x& src){
	buffer << "{ addr: " << src.address << ", fd: " << src.file_descriptor << ", dev: " << src.device << " }";
	return buffer;
}
