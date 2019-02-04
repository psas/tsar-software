#ifndef _LINK_LOGGER_H_
#define _LINK_LOGGER_H_

#include <fstream>                          // file io
#include <iostream>                         // cout
#include <cstring>                          // string
#include <thread>                           // threading
#include <mutex>                            // mutex
#include <string>                           // strings

#include "server.h"
#include "fixed_queue.h"
#include "client_command.h"
#include "sensor_data_frame.h"
#include "sequence_status.h"

#define FILENAME "saved_output/CM_data.txt" // output filename TODO: change this to add a time/date to name
#define LINK_LOGGER_DELAY 3                 // ms  
#define SEND_DATA_Q_LEN 250
#define CLIENT_COM_Q_LEN 250
#define STATUS 1                            // for flag in send_data
#define FRAME 0                             // for flag in send_data


/* POD for send queue. To reduce new and delete calls, this was used over a 
 * send_data base class with sensor_data_frame and sequence_status as derived classes being upcasted.
 * TODO: research into last statement
 */
struct send_data {
    sensor_data_frame sensor_frame;
    sequence_status seq_status;
    bool flag; // 0 for sensor, 1 for sequence
};


/* link_logger:
 * Acts as a wrapper class for the server class. The main of this class is to convert data 
 * given by the hardware_controller and sequencer classes into strings and passing them
 * to the server class to be sent. This will also convert recieved strings from client(s)
 * into a command class object for the sequencer to process. 
 * This will also save all strings made for later data/system analysis.
 */
class link_logger {
    public:
        link_logger();
        ~link_logger();

        int send(const sequence_status &);
        int send(const sensor_data_frame &);
        int recv(client_command &);
        void kill_driver();
        void driver_loop();
    private:
        void make_send_string(send_data & in, std::string & out);
        int save(std::string &) const;
        int data_changed(send_data) const;
        void start_server();

        std::thread serv_thread;
        server serv;
        fixed_queue<send_data> send_q;
        fixed_queue<client_command> recv_q;
        struct timespec driver_delay;
        struct send_data last_out_data;    // last frame sent to server
        struct send_data last_in_data;     // last frame created, used in send() to reduce contructor/decontructor calls
        bool driver_running;
        std::mutex ll_mutex;
};

#endif
