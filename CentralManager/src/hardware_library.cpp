#include "hardware_library.h"

int32_t hardware_library::
MPL3115A2_setup(const uint32_t reg) {
    int32_t fd = wiringPiI2CSetup(reg);

    if(fd != -1) { // setup did not fail
        // set necessary registers
        wiringPiI2CWriteReg8(fd,0x13, 0x07);
        wiringPiI2CWriteReg8(fd,0x26, 0x39);
    }

    return fd;
}


// returns pressure (in kPa) from mpl3115a2
float hardware_library::
MPL3115A2_pres(const uint32_t fd) {
    int32_t msb;                    // pressure most significant bits
    int32_t csb;                    // pressure central significant bits
    int32_t lsb;                    // pressure least significant bits
    float pres;                     // return value 

    msb = wiringPiI2CReadReg8(fd,0x01);
    csb = wiringPiI2CReadReg8(fd,0x02);
    lsb = wiringPiI2CReadReg8(fd,0x03);

    if(msb == -1 || csb == -1 || lsb == -1) {
        std::cout << "MPL3115A2 Pressure Fail" << std::endl;
        return -1;
    }

    pres = (float)(((msb * 65536) + (csb * 256) + (lsb & 0xF0)) / 16);
    pres = pres / 4.0 / 1000.0;

    return pres;
}


// returns temperature (in C) from mpl3115a2
float hardware_library::
MPL3115A2_temp(const uint32_t fd) {
    int32_t msb;                    // temperature most significant bits
    int32_t lsb;                    // temperature least significant bits
    float temp;                     // return value

    msb = wiringPiI2CReadReg8(fd,0x04);
    lsb = wiringPiI2CReadReg8(fd,0x05);

    if(msb == -1 || lsb == -1) {
        std::cout << "MPL3115A2 Temperature Fail" << std::endl;
        return -1;
    }

    temp = (float)((msb * 256) + (lsb & 0xF0)) / 256.0;

    return temp;
}

int32_t hardware_library::
random_int32_t() { return rand() % 100; }


float hardware_library::
random_float() { return (float)(rand() % 100); }

