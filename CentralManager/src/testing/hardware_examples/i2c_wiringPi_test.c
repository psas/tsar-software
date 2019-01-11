#include <stdio.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>

int main (int argc, char *argv[])
{
    int fd, data;

    wiringPiSetup();
    fd=wiringPiI2CSetup(0x68);

    if(fd==-1) {
        printf("Can't setup the I2C device\n");
        return -1;
    }
    else {
        data=wiringPiI2CWriteReg8(fd,0x01,0x90);
        for (;;) {
            data=wiringPiI2CReadReg8(fd,0x0);
            if(data==-1) {
                printf("No data\n");
            }
            else {
                printf("data=%d\n", data);
            }
            sleep(1);
        }
    }

    return 0;
}
