// gcc -Wall uart.c -lwiringPi

#include <stdio.h>
#include <wiringPi.h>
#include <wiringSerial.h>

#define BUAD_RATE 115200
#define PATH "/dev/ttyAMA0"
 
int main() {
    int fd;

    if(wiringPiSetup() < 0)
        return 1;
    if((fd = serialOpen(PATH,BUAD_RATE)) < 0)
        return 1;

    printf("serial test start ...\n");

    serialPrintf(fd,"Hello World!!!\n");a

    while(1) {
        serialPutchar(fd,serialGetchar(fd));
    }  

    serialClose(fd);
    return 0;
}
