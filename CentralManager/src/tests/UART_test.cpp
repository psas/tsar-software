#include <iostream>
#include <wiringSerial.h>
#include <chrono>
#include <thread>
#include <cstring>

#define BUAD_RATE 9600                  // UART buad rate
#define UART_PATH "/dev/ttyACM0"        // UART path
#define AC_TO_CM_LEN 16                 // message length in Bytes
#define CM_TO_AC_LEN 12                 // message length in Bytes
#define HB_TIME_MS 300                  // send heartbeat time in milliseconds

using namespace std;


bool valid_message(char * AC_message) {
    unsigned int cs = 0;
    for(int i=0; i<AC_TO_CM_LEN-4; ++i)
        cs += (AC_message[i] * AC_message[i] * AC_message[i]) % 251;
    return !memcmp(&AC_message[AC_TO_CM_LEN-4], &cs, sizeof(unsigned int));
}

int main() {
    char * AC_message = new char[AC_TO_CM_LEN];
    
    // 0  1  2  3  4  5  6  7  8  9 10 11
    //0a ff 0f 00 00 00 00 00 a7 01 00 00

    char * message = new char[CM_TO_AC_LEN];
    message[0] = '\n';
    message[1] = 0xff;
    message[2] = 0x0f;
    message[3] = 0x00; // sensors 
    message[4] = 0x00;
    message[5] = 0x00;
    message[6] = 0x00;
    message[7] = 0x00; 
    message[8] = 0x00; // checksum
    message[9] = 0x00;
    message[10] = 0x00;
    message[11] = 0x00;

    unsigned int cs = 0;
    for(int i=0; i<CM_TO_AC_LEN-4; ++i)
        cs += (message[i] * message[i] * message[i]) % 251;
    memcpy(&message[CM_TO_AC_LEN-4], &cs, sizeof(unsigned int));

    int a;
    int fd;
    bool AC_connected = false;
    auto current_time = std::chrono::system_clock::now();
    auto _next_heartbeat_time = current_time - std::chrono::milliseconds(HB_TIME_MS);

    while(1) { 
        // open uart connection if one is not open
        if(AC_connected == false) { 
            fd = serialOpen(UART_PATH, BUAD_RATE);
            if(fd == -1) { // no connection availible
                cout << "No UART connection" << endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                continue;
            }
            else { // connection openned
                AC_connected = true;
            }
        }

        // send hearbeat every HB_TIME_MS milliseconds
        current_time = std::chrono::system_clock::now();
        if(current_time >= _next_heartbeat_time) {
            for(int i=0; i<CM_TO_AC_LEN; ++i) {
                serialPutchar(fd, message[i]);
            }
            _next_heartbeat_time = current_time + std::chrono::milliseconds(HB_TIME_MS);
        }

        // check for disconnect
        if(serialDataAvail(fd) == -1) {
            cout << "error, closing connection" << endl;
            serialClose(fd);
            AC_connected = false;
        }
        else {
            // remove all data upto a '\n'
            while(serialDataAvail(fd) > 2*AC_TO_CM_LEN) {
                if(serialGetchar(fd) == '\n' && serialDataAvail(fd) >= AC_TO_CM_LEN) {
                    // grab a message
                    if(serialDataAvail(fd) >= AC_TO_CM_LEN-1) {
                        cout << hex << 0xa << '\t';
                        AC_message[0] = '\n';
                        for(int i=1; i<AC_TO_CM_LEN; ++i) {
                            AC_message[i] = serialGetchar(fd);
                            cout << hex << (int)AC_message[i] << '\t';
                        }
                        cout << "valid: " << valid_message(AC_message);
                        cout << endl;
                    }
                    break;
                }
            }
        }

        // sleep
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    
    serialClose(fd);
    return 1;
}
