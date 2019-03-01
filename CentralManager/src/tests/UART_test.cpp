#include <iostream>
#include <wiringSerial.h>           // raspberry pi uart
#include <chrono>
#include <thread>
#include <cstring>

#define BUAD_RATE 9600                  // UART buad rate
#define UART_PATH "/dev/ttyACM0"        // UART path

using namespace std;


int main() {
    // setup uart Actuator Controller connection
    int fd = serialOpen(UART_PATH, BUAD_RATE);
    
    // 0  1  2  3  4  5  6  7  8  9 10 11
    //0a ff 0f 00 00 00 00 00 a7 01 00 00
    //0a ff 0f 01 00 00 00 00 a8 01 00 00

    char * x = new char[12];
    x[0] = '\n';
    x[1] = 0xff;
    x[2] = 0x0f;
    x[3] = 0x00;
    x[4] = 0x00;
    x[5] = 0x00;
    x[6] = 0x00;
    x[7] = 0x00;
    x[8] = 0x00;
    x[9] = 0x00;
    x[10] = 0x00;
    x[11] = 0x00;

    unsigned int cs = 0;
    for(int i=0; i<8; ++i) {
        cs += (x[i] * x[i] * x[i]) % 251;
    }
    cout << hex << cs << '\n' << endl;

    memcpy(&x[8], &cs, sizeof(unsigned int));


    for(int i=0; i<12; ++i) {
        cout << hex << (int)x[i] << endl;
    }
    cout << endl;

    int a;

    while(1) { 
        for(int i=0; i<12; ++i) {
            serialPutchar(fd, x[i]);
        }

        if(serialDataAvail(fd) < 0) {
            cout << "error" << endl;
        }
        while(serialDataAvail(fd) > 0) {
            cout << hex << serialGetchar(fd) << endl;
        }
        cout << endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
    serialClose(fd);
    return 1;
}
