#include "hardware_library.h"

#ifndef LIVE_DATA
int hardware_library::random_int(int addr) { return rand() % 100; }
float hardware_library::random_float(int addr) { return (float)(rand() % 100); }
#endif
