#ifndef _LINK_LOGGER_H_
#define _LINK_LOGGER_H_

#include <fstream>                          // file io
#include <iostream>                         // cout
#include <cstring>                          // string
#include <thread>                           // threading

#include "../thirdparty/rapidjson/document.h"       // rapidjson
#include "../thirdparty/rapidjson/writer.h"         // rapidjson
#include "../thirdparty/rapidjson/stringbuffer.h"   // rapidjson

#include "server.h"
#include "send_data.h"
#include "fixed_queue.h"
#include "client_command.h"

#define FILENAME "saved_output/CM_data.txt" // output filename
#define LINK_LOGGER_DELAY 3                 // ms  
#define SEND_DATA_Q_LENGTH 250

/* link_logger:
 * Acts as a wrapper class for the server class. This class main jobs is to 
 * convert sensor_frame & sequencer_status structs into a string and pass
 * on the new string to the server class to be sent. This will also handle 
 * recieved strings by converting them into command struct for the sequencer
 * to process. This will also save all strings made for later data/system 
 * analysis.
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
        fixed_queue<struct send_data> send_q;

        struct timespec driver_delay;
        struct send_data last_frame; // last input data
        int sensor_frame_size; // sensor data frame struct size
        int send_data_size; // send data frame struct size
        int seq_status_size; // sequencer status struct size
        bool driver_running;
};
#endif
