#include "hardware_library.h"

#ifdef LIVE_DATA
int hardware_library::
mcp3424(int addr, int reg) {
    int ri = wiringPiI2CRead(addr);
    std::cout << ri << std::endl;
    return ri;
}
#else
int hardware_library::
random_int() { return rand() % 100; }

float hardware_library::
random_float() { return (float)(rand() % 100); }
#endif

