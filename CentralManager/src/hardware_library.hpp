#ifndef _HARDWARE_LIBRARY_H_
#define _HARDWARE_LIBRARY_H_

#include <iostream>                 // cout, cerr
#include <cstdint>                  // uint_t
#include <wiringPiI2C.h>            // raspberry pi I2C
#include <iostream>                 // cerr
#include <cstring>                  // memcpy
#include <array>                    // array

#include "sensor_data_frame.h"      // AC_data_frame

// UART params
#define AC_TO_CM_LEN 16                 // message length in Bytes
#define CM_TO_AC_LEN 7                  // message length in Bytes

#define AC_CMD_DO_NOTHING 0
#define AC_CMD_PANIC 1

#define AC_FM_NO_FAILURE 0
#define AC_FM_OTHER_FAILURE 255


/*
 * This contain all hardware files for I2C and UART
 */
namespace hardware_library {
    /* MPL3115A2 pressure/temperature sensor 
     * only used for testing i2c and as an example until
     * TSAR hardware is ready for testing
     */
    int MPL3115A2_setup(const int & reg) {
        int fd = wiringPiI2CSetup(reg);

        if(fd != -1) { // setup did not fail
            // set necessary registers
            wiringPiI2CWriteReg8(fd, 0x13, 0x07);
            wiringPiI2CWriteReg8(fd, 0x26, 0x39);
        }

        return fd;
    }


    // returns pressure (in kPa) from mpl3115a2
    double MPL3115A2_pres(const int & fd) {
        int32_t msb;                    // pressure most significant bits
        int32_t csb;                    // pressure central significant bits
        int32_t lsb;                    // pressure least significant bits
        double pres;                     // return value 

        msb = wiringPiI2CReadReg8(fd, 0x01);
        csb = wiringPiI2CReadReg8(fd, 0x02);
        lsb = wiringPiI2CReadReg8(fd, 0x03);

        if(msb == -1 || csb == -1 || lsb == -1) { // hardware read fail 
            //std::cout << "MPL3115A2 Pressure Fail" << std::endl;
            return -1;
        }

        // math on bits as defined in mpl3115a2 datasheet for pressure value
        pres = (double)(((msb * 65536) + (csb * 256) + (lsb & 0xF0)) / 16);
        pres = pres / 4.0 / 1000.0;

        return pres;
    }


    // returns temperature (in C) from mpl3115a2
    double MPL3115A2_temp(const int & fd) {
        int32_t msb;                    // temperature most significant bits
        int32_t lsb;                    // temperature least significant bits
        double temp;                     // return value

        msb = wiringPiI2CReadReg8(fd, 0x04);
        lsb = wiringPiI2CReadReg8(fd, 0x05);

        if(msb == -1 || lsb == -1) {
            //std::cout << "MPL3115A2 Temperature Fail" << std::endl;
            return -1;
        }

        // math on bits as defined in mpl3115a2 datasheet for temperature value
        temp = (double)((msb * 256) + (lsb & 0xF0)) / 256.0;

        return temp;
    }

    // *************************************************************************************************

    namespace {
        // Checks and reads message from uart buffer
        int read_uart_message(std::array<uint8_t, AC_TO_CM_LEN> & message, int fd) {
            int rv = 0; 

            int len = serialDataAvail(fd);
            if(len > 0) { // has new message
                if(len >= AC_TO_CM_LEN) { // full message avaible
                    for(unsigned int i=0; i<AC_TO_CM_LEN; ++i)
                        message[i] = serialGetchar(fd);
                    rv = 1;
                }
                else { // incomplete message
                    std::cout << "uart message length " << std::endl; // TODO log this
                    rv = -1;
                }
            }
            else if (len == -1) { // error
                std::cout << "serialDataAvail error" << std::endl; // TODO log this
                rv = -1;
            }
            return rv;
        }


        // parses the message's from the Actuator Controller's checksum for correctness
        bool check_checksum(const std::array<uint8_t, AC_TO_CM_LEN> & message) const { 
            uint32_t value = 0;

            for(unsigned int i=0; i<AC_TO_CM_LEN-4; ++i) // dont check 4 checksum bytes
                value += ((message[i] * message[i] * message[i]) % 251);

            uint32_t checksum;
            std::memcpy(&checksum, &message[AC_TO_CM_LEN-4], 4);
            //checksum |= (message[AC_TO_CM_LEN-4] << 24);
            //checksum |= (message[AC_TO_CM_LEN-3] << 16);
            //checksum |= (message[AC_TO_CM_LEN-2] << 8);
            //checksum |= message[AC_TO_CM_LEN-1];

            if(value == checksum)
                return true;
            return false;
        }


        // store uart data into frame if it is valid
        int process_uart_message(const std::array<uint8_t, AC_TO_CM_LEN> & message, AC_data_frame & data) {
            if(message[0] != '\n')
                return -1;
            if(check_checksum(message) == false) // TODO count this?
                return -1;

            data.AC_next_failure_mode = message[1];
            data.AC_failure_mode = message[2];
            data.AC_failure_cause = message[3];
            data.AC_consecutive_checksum_errors = message[4];
            data.AC_lifetime_checksum_errors = message[5];
            data.AC_sensor1 = message[6];
            data.AC_sensor2 = message[7];
            data.AC_sensor3 = message[8];
            data.AC_sensor4 = message[9];
            std::memcpy(&data.AC_Sensor5, &message[10], 2);

            return 1;
        }


        /* 
         * this will generate a checksum for the message to the Actuator Controller 
         * and insert it into that message
         */
        void generate_checksum(std::array<uint8_t, CM_TO_AC_LEN> & message) { 
            uint32_t checksum = 0;

            for(unsigned int i=0; i<AC_TO_CM_LEN-4; ++i) // dont check 4 checksum bytes
                checksum += ((message[i] * message[i] * message[i]) % 251);

            std::memcpy(&message[AC_TO_CM_LEN-4], &checksum, 4);

            //message[AC_TO_CM_LEN-4] = (uint8_t)((checksum >> 24) & 0xFF000000);
            //message[AC_TO_CM_LEN-3] = (uint8_t)((checksum >> 16) & 0x00FF0000);
            //message[AC_TO_CM_LEN-2] = (uint8_t)((checksum >> 8) & 0x0000FF00);
            //message[AC_TO_CM_LEN-1] = (uint8_t)(checksum & 0x000000FF);

            return;
        }


        // TODO change default vs sequencer command
        int make_uart_message(std::array<uint8_t, CM_TO_AC_LEN> & message) {
            message[0] = '\n';
            message[1] = AC_FM_NO_FAILURE;      // Next Failure Mode
            message[2] = AC_CMD_DO_NOTHING;     // Command
            message[3] = 1;                     // sensor 1
            message[4] = 1;                     // sensor 2
            message[5] = 1;                     // sensor 3
            message[6] = 1;                     // sensor 4
            message[7] = 1;                     // sensor 5
            generate_checksum(message); // handles inserting checksum into message
            return 1;
        }
    }


    int uart_update(AC_data_frame & data, const int & fd) {
        std::array<uint8_t, AC_TO_CM_LEN> message;

        int rv = read_uart_message(message, fd);
        if(rv <= 0) // no new message or error
            return rv;

        if(process_uart_message(message, data) == -1) // no new message or error
            return -1;


        /* if read_uart_message() and process_uart_message return 1 
         * data is valid and over   riden
         */
        return 1;
    }

    int uart_send_command(int fd) {
        std::array<uint8_t, CM_TO_AC_LEN> message; 
        int rv = make_uart_message(message);
        if(rv != 1)
            return 0;

        serialPuts(fd, &message[0]);
        return 1;
    }
}

#endif // _HARDWARE_LIBRARY_H_
