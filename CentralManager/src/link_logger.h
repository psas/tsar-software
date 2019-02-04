#ifndef _LINK_LOGGER_H_
#define _LINK_LOGGER_H_

#include <fstream>                          // file io
#include <iostream>                         // cout
#include <cstring>                          // string
#include <thread>                           // threading
#include <mutex>                            // mutex

#include "server.h"
#include "send_data.h"
#include "fixed_queue.h"
#include "client_command.h"

#define FILENAME "saved_output/CM_data.txt" // output filename
#define LINK_LOGGER_DELAY 3                 // ms  
#define SEND_DATA_Q_LENG 250
#define CLIENT_COM_Q_LEN 250

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
        int save(const char *) const;
        int make_send_string(const send_data &, char *);
        int make_send_string(const sequence_status &, char *);
        int make_send_string(const sensor_data_frame &, char *);
        int make_command_data(const char *, client_command &) const;
        int data_changed(send_data) const;
        void start_server();

        std::thread * serv_thread;
        server serv;
        fixed_queue<send_data> send_q;
        fixed_queue<client_command> recv_q;

        struct timespec driver_delay;
        struct send_data last_frame; // last input data
        int sensor_frame_size; // sensor data frame struct size
        int send_data_size; // send data frame struct size
        int seq_status_size; // sequencer status struct size
        bool driver_running;
        send_data new_data;
        std::mutex ll_mutex;
};
#endif
