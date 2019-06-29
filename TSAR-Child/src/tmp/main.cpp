#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <unistd.h>

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

int main(){
	char buffer[3];
	int address = 0x68;
	int file = open("/dev/i2c-1", O_RDWR);

	buffer[0] = address;

	ioctl(file, I2C_SLAVE, address);

	while(1){
		printf("Reading the first three bytes from reg %i\n", address);
		read(file, buffer, 3);		
		usleep(1000000);
	}

	return 0;
}
