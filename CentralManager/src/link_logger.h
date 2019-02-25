#ifndef _LINK_LOGGER_H_
#define _LINK_LOGGER_H_

#include <fstream>                          // file io
#include <string>                           // strings
#include <chrono>                           // time
#include <thread>                           // sleep_for

#include "main_class.h"
#include "server.h"
#include "fixed_queue.hpp"
#include "client_command.h"
#include "hardware_data_frame.h"
#include "sequencer_status.h"

#define FILENAME "CM_data.txt"              // output filename TODO: change this to add a time/date to name
#define LINK_LOGGER_DELAY 50                // milliseconds  
#define SEND_DATA_Q_LEN 500
#define CLIENT_COM_Q_LEN 100
#define SEQ 1                               // for flag in send_data
#define HDW 0                               // for flag in send_data
#define FULL_SEND 50                        // every x loops, send all data in message


/* POD for send queue. To reduce new and delete calls, this was used over a 
 * send_data base class with hardware_data_frame and sequencer_status as derived 
 * classes being upcasted.
 */
struct send_data {
    hardware_data_frame hardware_data;
    sequencer_status seq_status;
    bool flag; // 0 for hardware data frame, 1 for sequenecer status
};


/* link_logger:
 * Acts as a wrapper class for the server class. The main of this class is to convert data 
 * given by the hardware_controller and sequencer classes into strings and passing them
 * to the server class to be sent. This will also convert recieved strings from client(s)
 * into a command class object for the sequencer to process. 
 * This will also save all strings made for later data/system analysis.
 */
class link_logger : public main_class {
    public:
        link_logger(std::shared_ptr<server> input_server);

        int send(const sequencer_status &);
        int send(const hardware_data_frame &);
        int recv(client_command &);
        void driver_loop();
    private:
        void make_send_string(send_data & in, std::string & out);
        void make_send_string_all(send_data & in, std::string & out);
        int save(std::string &) const;
        int data_changed(send_data) const;

        std::shared_ptr<server> serv;
        fixed_queue<send_data> send_q;
        fixed_queue<client_command> recv_q;

        // used to remove data that has not changed from being added to json string
        hardware_data_frame last_out_hdw_data;
        sequencer_status last_out_seq_status;

        // used to remove data that has not changed from being enqueued
        hardware_data_frame last_in_hdw_data;
        sequencer_status last_in_seq_status;
};

#endif
