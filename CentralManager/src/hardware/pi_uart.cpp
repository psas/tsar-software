#include "pi_uart.h"


// default construtor
pi_uart::pi_uart(const char * path, const int & baud) _path(path), _baud(baud), _connected(false) {}


// closes the uart connecction if open;
pi_uart::~pi_uart() {
    if(_connected == true)
        serialClose(_fd);
}


/* This start the uart connection if not already started. Also this will send the entire
 * vector of char to the given device set in the constructor. It will add a '\n' before
 * the message and a 4 byte checksum after the message. The checksum include the '\n'.
 */
bool pi_uart::
send(std::vector<char> & message) {
    // open uart connection if one is not open
    if(_connected == false) { 
        _fd = serialOpen(&_path[0], _baud);
        if(_fd == -1) // no connection availible
            return 0;
        else // connection openned
            _connected = true;
    }
    
    char message_checksum[4];
    uint32_t cs = make_checksum(message); 
    std::memcpy(&message_checksum, &cs, sizeof(uint32_t));

    serialPutchar(_fd, '\n');                       // send '\n'
    for(unsigned int i=0; i<message.size(); ++i)    // send message
        serialPutchar(_fd, message[i]);
    for(unsigned int i=0; i<4; ++i)                 // send checksum
        serialPutchar(_fd, message_checksum[i]);
    return 1;
}


/* This will close the uart message if it errors. Also, this will send the entire vector
 * of char to the given device set in the constructor It will remove the '\n' before the
 * message and the 4 byte checksum after the message. The checksum include the '\n'.
 */
bool pi_uart::
read(std::vector<char> & message) {
    if(_connected == false)
        return 0;

    char message_checksum[4];
    bool rv = false;

    // check for disconnect
    if(serialDataAvail(_fd) == -1) { // AC disconnected
        serialClose(_fd);
        _connected = false;
    }
    else { // read a uart mess if one is availabe
        while(serialDataAvail(_fd) > 2*message.size()) { // there is full message to grab
            if(serialGetchar(_fd) == '\n' && serialDataAvail(_fd) >= message.size()+4) { // remove all data to '\n'
                for(int i=0; i<message.size(); ++i)         // get message
                    message[i] = serialGetchar(_fd);
                for(int i=0; i<4; ++i)                      // get checksum
                    message_checksum[i] = serialGetchar(_fd);

                // check checksum
                uint32_t cs = make_checksum(message); 
                if(std::memcmp(&cs, &message_checksum, sizeof(uint32_t)) == 0)
                    rv = true;
                
                break;
            }
        }
    }

    return rv;
}


/* Calculates checksum for message. This expects the message not the entire package 
 * (no starting '\n' or 4 byte checksum space at end).
 */
uint32_t
make_checksum(const std::vector<char> & message) { 
    uint64_t checksum = ('\n' * '\n' * '\n') % 251;

    for(unsigned int i=0; i<message.size(); ++i)
        checksum += ((message[i] * message[i] * message[i]) % 251);

    return (uint32_t)checksum;
}


// returns if the device is connected or not
bool pi_uart::
status() const {
    return _connected;
}
