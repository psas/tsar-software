#include <wiringSerial.h>
#include <vector>
#include <string>
#include <cstring>


/* This is a RAII class to handle uart message for a Raspbery Pi and another usb device. 
 * It expect all received uart message to use format of '\n' fallow by the message 
 * fallow by a 4 byte checksum. It will creat the '\n' and the checksum  for the any 
 * messages sent. Both send and read expect a vector of chars and will send/read data
 * for the entire vector.
 */
class pi_uart {
    public:
        pi_uart();
        ~pi_uart();
        bool read(std::vector<char> & message);
        bool send(const std::vector<char> & message);
        bool status() const;
    private:
        std::string _path;
        int _baud; 
        bool _connected;
        int _fd;
};
