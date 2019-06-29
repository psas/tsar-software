#include "mcp342x.h"

//
// MCP342X Implementation
//
mcp342x::mcp342x() : address(0), file_descriptor(0), device(NULL), register_size(0), register_data(NULL){}
mcp342x::mcp342x(const int address, const char* input_device, const int register_size) : address(address), register_size(register_size){
  file_descriptor = open(input_device, O_RDWR);

  if(file_descriptor < 0){
    std::cerr << "Device " << input_device << " couldn't be opened!" << std::endl << "Gracefully exiting" << std::endl;
    exit(0);
  }

  register_data = new char[10];
  ioctl(file_descriptor, I2C_SLAVE, address);
}

mcp342x::~mcp342x(){
  std::cout << "Closing the pipe: " << file_descriptor << std::endl;
  close(file_descriptor);
}

const int mcp342x::read(){
  return read(register_size, register_data);
}

const int mcp342x::read(const int size, const char* data){
  // return (::read(file_descriptor, data, size));
  return -1;
}
