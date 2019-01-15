#include "hardware_library.h"

#ifdef LIVE_DATA
int hardware_library::
MPL3115A2_setup(int reg) {
    int fd = wiringPiI2CSetup(reg);
    if(fd == -1) {
        std::cout << "Can't setup the I2C device\n" << std::endl;
        return -1;
    }

    wiringPiI2CWriteReg8(fd,0x13, 0x07);
    wiringPiI2CWriteReg8(fd,0x26, 0x39);

    return fd;
}


float hardware_library::
MPL3115A2_pres(int fd) {
    int msb;                    // pressure most significant bits
    int csb;                    // pressure central significant bits
    int lsb;                    // pressure least significant bits
    float pres;                 // return value (pressure in kPa)

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


float hardware_library::
MPL3115A2_temp(int fd) {
    int msb;                    // temperature most significant bits
    int lsb;                    // temperature least significant bits
    float temp;                 // return value (temperature in C)

    msb = wiringPiI2CReadReg8(fd,0x04);
    lsb = wiringPiI2CReadReg8(fd,0x05);

    if(msb == -1 || lsb == -1) {
        std::cout << "MPL3115A2 Temperature Fail" << std::endl;
        return -1;
    }

    temp = (float)((msb * 256) + (lsb & 0xF0)) / 256.0;

    return temp;
}
#else
int hardware_library::
random_int() { return rand() % 100; }


float hardware_library::
random_float() { return (float)(rand() % 100); }
#endif

