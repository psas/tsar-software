#include "link_logger.h"


// constructor
link_logger::
link_logger(std::shared_ptr<server> input_server) : serv(input_server), 
        send_q(SEND_DATA_Q_LEN), recv_q(CLIENT_COM_Q_LEN) {}


// server wrapper for link thread
void link_logger::
driver_loop() {
    std::string temp_string;
    send_data temp_send_data;
    unsigned int i = 0;

    _driver_running = true;
    while(_driver_running) {
        _mutex.lock();
        
        while(send_q.status() > 0) {
            send_q.dequeue(temp_send_data);
            if(data_changed(temp_send_data)) {
                ++i;
                continue;
            }

            // TODO get commands from server recv queue 

            // every x loop, send a message with all the data / status 
            if(i < FULL_SEND) {
                make_send_string(temp_send_data, temp_string);
                ++i;
            }
            else { 
                make_send_string_all(temp_send_data, temp_string);
                i = 0;
            }

            if(serv != nullptr)
                serv->send_string(temp_string);
            save(temp_string);

            last_out_data = temp_send_data;
        }
        
        _mutex.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(LINK_LOGGER_DELAY));
    }
    return;
}


// TODO return value is wrong
// compaires data is see if it is the same
int link_logger::
data_changed(send_data input) const {
    if(input.flag == HDW)
        return (input.hardware_data == last_out_data.hardware_data);
    return (input.seq_status == last_out_data.seq_status);
}


int link_logger::
send(const sequencer_status & input) {
    send_data temp;

    // update new send data
    temp.seq_status = input;
    temp.flag = SEQ; 

    // send data
    return send_q.enqueue(temp); 
}


int link_logger::
send(const hardware_data_frame & input) {
    send_data temp;

    // make data to add to queue
    temp.hardware_data = input;
    temp.flag = HDW; 

    // send data
    return send_q.enqueue(temp); 
}


// save string
int link_logger::
save(std::string & input_string) const {
    std::ofstream file; // file for saving data on rasp pi
    file.open(FILENAME, std::fstream::app); //append to file
    file << input_string;
    file << '\n';
    file.close();
    return 1;
}


// Receive wrapper, to convert string from server to a command struct for processing 
int link_logger::
recv(client_command & output) {
    std::string temp_string;

    if(serv->recv_string(temp_string) == 0) // no new string
        return 0;

    output.make_command_data(temp_string);
    return 1;
}


// makes a string with only data that has changed 
void link_logger::
make_send_string(send_data & in, std::string & out) {
    if(in.flag == HDW)
        in.hardware_data.make_JSON_diff(out, last_out_data.hardware_data);
    else
        in.seq_status.make_JSON_diff(out, last_out_data.seq_status);
    return;
}


// make a string with all data
void link_logger::
make_send_string_all(send_data & in, std::string & out) {
    if(in.flag == HDW)
        in.hardware_data.make_JSON(out);
    else
        in.seq_status.make_JSON(out);
    return;
}

