#include "uart_library.h"

/*
 * This contain all UART functions
 */
namespace uart_library {
namespace {
    // Checks and reads message from uart buffer
    int
    read_uart_message(std::string & message, const int & fd) {
        int rv = 0; 

        int len = serialDataAvail(fd);
        if(len > 0) { // has new message
            if(len >= AC_TO_CM_LEN) { // full message avaible
                for(unsigned int i=0; i<AC_TO_CM_LEN; ++i) {
                    message[i] = serialGetchar(fd);
                    std::cout  << std::hex << (int)message[i];
                }
                std::cout << std::endl;
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
    bool
    check_checksum(const std::string & message) { 
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
    process_uart_message(const std::string & message, AC_data_frame & data) {
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


    /* 
     * this will generate a checksum for the message to the Actuator Controller 
     * and insert it into that message
     */
    void
    generate_checksum(std::string & message) { 
        uint32_t checksum = 0;

        for(unsigned int i=0; i<AC_TO_CM_LEN-4; ++i) // dont check 4 checksum bytes
            checksum += ((message[i] * message[i] * message[i]) % 251);

        std::memcpy(&message[AC_TO_CM_LEN-4], &checksum, 4);

        return;
    }


    // TODO change default vs sequencer command
    int
    make_uart_message(std::string & message) {
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


int
read(AC_data_frame & data, const int & fd) {
    std::string message('0', AC_TO_CM_LEN); //make a placeholder for the data in the container to reduce dynamic calls

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

int 
send_default(const int & fd) {
    std::string message('0', CM_TO_AC_LEN); //make a placeholder for the data in the container to reduce dynamic calls
    if(!make_uart_message(message))
        return 0;

    serialPuts(fd, &message[0]);
    return 1;
}

}

