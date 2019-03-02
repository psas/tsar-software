#include "uart_library.h"

/*
 * This contain all UART functions
 */
namespace uart_library {
namespace {
    // Checks and reads message from uart buffer
    int
    read_uart_message(std::array<char, AC_TO_CM_LEN> & message, const int & fd) {
        int rv = 0; 

        int len = serialDataAvail(fd);
        if(len > 0) { // has new message
            if(len >= AC_TO_CM_LEN) { // full message avaible
                while(serialGetchar(fd) != '\n' && serialDataAvail(fd)); // remove all data upto '/n'
                message[0] = '\n';

                for(unsigned int i=1; i<AC_TO_CM_LEN; ++i)
                    message[i] = serialGetchar(fd);
                rv = 1;
            }
            else { // incomplete message
                rv = 0;
            }
        }
        else if (len == -1) { // error
            //std::cout << "serialDataAvail error" << std::endl; // TODO log this
            rv = -1;
        }
        return rv;
    }


    // parses the message's from the Actuator Controller's checksum for correctness
    bool
    check_checksum(const std::array<char, AC_TO_CM_LEN> & message) { 
        uint32_t value = 0;

        for(unsigned int i=0; i<AC_TO_CM_LEN-4; ++i) // dont check 4 checksum bytes
            value += ((message[i] * message[i] * message[i]) % 251);

        uint32_t checksum;
        std::memcpy(&checksum, &message[AC_TO_CM_LEN-4], 4);

        if(value == checksum)
            return true;
        return false;
    }


    // store uart data into frame if it is valid
    int
    process_uart_message(const std::array<char, AC_TO_CM_LEN> & message, AC_data_frame & data) {
        if(message[0] != '\n')
            return -1;
        if(check_checksum(message) == false) // TODO count this?
            return -1;

        data.next_failure_mode = message[1];
        data.failure_mode = message[2];
        data.failure_cause = message[3];
        data.consecutive_checksum_errors = message[4];
        data.lifetime_checksum_errors = message[5];
        data.sensor1 = message[6];
        data.sensor2 = message[7];
        data.sensor3 = message[8];
        data.sensor4 = message[9];
        std::memcpy(&data.sensor5, &message[10], 2);

        return 1;
    }


    /* This will generate a checksum for the message to the Actuator Controller 
     * and insert it into that message
     */
    void
    generate_checksum(std::array<char, CM_TO_AC_LEN> & message) { 
        uint32_t checksum = 0;

        for(unsigned int i=0; i<CM_TO_AC_LEN-4; ++i) // dont check 4 checksum bytes
            checksum += ((message[i] * message[i] * message[i]) % 251);

        std::memcpy(&message[CM_TO_AC_LEN-4], &checksum, sizeof(uint32_t));

        return;
    }


    // TODO change default vs sequencer command
    void
    make_uart_message(std::array<char, CM_TO_AC_LEN> & message) {
        message[0] = '\n';
        message[1] = 0xff;                  // Next Failure Mode
        message[2] = 0x0f;                  // Command
        message[3] = 0;                     // sensor 1
        message[4] = 0;                     // sensor 2
        message[5] = 0;                     // sensor 3
        message[6] = 0;                     // sensor 4
        message[7] = 0;                     // sensor 5
        generate_checksum(message);         // handles inserting checksum into message
        return;
    }
}


int
read(AC_data_frame & data, const int & fd) {
    std::array<char, AC_TO_CM_LEN> message;

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

void 
send_default(const int & fd) {
    std::array<char, CM_TO_AC_LEN> message; //make a placeholder for the data in the container to reduce dynamic calls
    make_uart_message(message);
    for(unsigned int i=0; i<CM_TO_AC_LEN; ++i)
        serialPutchar(fd, message[i]);
    return;
}

}

