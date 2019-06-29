#ifndef TSAR_MCP342X
#define TSAR_MCP342X

#include <iostream>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>

//Raw Config Bit Definitions
#define CFG_RDY 1<<7
#define CFG_C1    1<<6
#define CFG_C0    1<<5
#define CFG_OC    1<<4
#define CFG_S1    1<<3
#define CFG_S0    1<<2
#define CFG_G1    1<<1
#define CFG_G0    1<<0

#define CFG_RDY 1<<7
#define CFG_C1 1<<6
#define CFG_C0 1<<5
#define CFG_OC 1<<4
#define CFG_S1 1<<3
#define CFG_S0 1<<2
#define CFG_G1 1<<1
#define CFG_G0 1<<0

#define CFG_CHAN1 0
#define CFG_CHAN2 CFG_C1
#define CFG_CHAN3 CFG_C0
#define CFG_CHAN4 (CFG_C0 |CFG_C1)

#define CFG_240SPS 0
#define CFG_60SPS CFG_S1
#define CFG_15SPS CFG_S0
#define CFG_3SPS (CFG_S0 | CFG_S1)

#define CFG_X1 0
#define CFG_X2 CFG_G1
#define CFG_X4 CFG_G0
#define CFG_X8 (CFG_G0 | CFG_G1)

#define CFG_DEFAULT (CFG_CHAN1 | CFG_240SPS | CFG_X1)

class mcp342x{
  private:
    int address;
    int file_descriptor;
    char* device;
    int register_size;
    char* register_data;

  public:
    // Constructor / destructor stuff
    mcp342x();
    mcp342x(const int, const char*, const int);
    ~mcp342x();

    // Function stuff
    const bool set_settings();
    const bool set_channel();
    void start_read();
    const bool is_ready();
    const int read();
    const int read(const int, const char*);
    const int write();
};

#endif
