#include <iostream>
#include <wiringPi.h>
#include <wiringPiI2C.h>


int MPL3115A2_setup(int reg);
float MPL3115A2_pres(int fd);
float MPL3115A2_temp(int fd);


int main () {
    int MPL3115A2_fd;
    float pres, alt, temp;

    wiringPiSetup();

    MPL3115A2_fd = MPL3115A2_setup(0x60);

    pres = MPL3115A2_pres(MPL3115A2_fd);
    temp = MPL3115A2_temp(MPL3115A2_fd);

    std::cout << "Pressure: " << pres << " kPa" << std::endl;
    std::cout << "Temperature: " << temp << " C" << std::endl;

    return 0;
}


int 
MPL3115A2_setup(int reg) {
    int fd = wiringPiI2CSetup(reg);
    if(fd == -1) {
        std::cout << "Can't setup the I2C device\n" << std::endl;
        return -1;
    }
    return fd;
}


float
MPL3115A2_pres(int fd) {
    int msb;                    // pressure most significant bits
    int csb;                    // pressure central significant bits
    int lsb;                    // pressure least significant bits
    float pres;                 // return value (pressure in kPa)

    msb = wiringPiI2CReadReg8(fd,0x01);
    if(msb == -1) {
        std::cout << "No data mpl3115a2 msb" << std::endl;
    }

    csb = wiringPiI2CReadReg8(fd,0x02);
    if(csb == -1) {
        std::cout << "No data mpl3115a2 csb" << std::endl;
    }

    lsb = wiringPiI2CReadReg8(fd,0x03);
    if(lsb == -1) {
        std::cout << "No data mpl3115a2 lsb" << std::endl;
    }

    pres = (float)(((msb * 65536) + (csb * 256) + (lsb & 0xF0)) / 16);
    pres = pres / 4.0 / 1000.0;

    return pres;
}


float
MPL3115A2_temp(int fd) {
    int msb;                    // temperature most significant bits
    int lsb;                    // temperature least significant bits
    float temp;                 // return value (temperature in C)

    msb = wiringPiI2CReadReg8(fd,0x04);
    if(msb == -1) {
        std::cout << "No data mpl3115a2 msb" << std::endl;
    }

    lsb = wiringPiI2CReadReg8(fd,0x05);
    if(lsb == -1) {
        std::cout << "No data mpl3115a2 lsb" << std::endl;
    }

    temp = (float)((msb * 256) + (lsb & 0xF0)) / 256.0;

    return temp;
}
