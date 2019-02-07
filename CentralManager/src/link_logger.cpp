#include "link_logger.h"


// constructor
link_logger::
link_logger(std::shared_ptr<server> input_server) : serv(input_server), 
        send_q(SEND_DATA_Q_LEN), recv_q(CLIENT_COM_Q_LEN), driver_running(false) {}


// server wrapper for link thread
void link_logger::
driver_loop() {
    std::string temp_string;
    send_data temp_send_data;

    driver_running = true;
    while(driver_running) {
        while(send_q.dequeue(temp_send_data) && data_changed(temp_send_data)) {
                // if there is temp data and it's differenet than last 
                make_send_string(temp_send_data, temp_string);
                serv->send_string(temp_string);
                save(temp_string);
        }
        std::this_thread::sleep_for(std::chrono::microseconds(LINK_LOGGER_DELAY));
    }
    return;
}


// kill server driver wrapper
void link_logger::
stop_driver() { 
    ll_mutex.lock();
    driver_running = false;
    ll_mutex.unlock();
    return;
}

// compaires data is see if it is the same
int link_logger::
data_changed(send_data input) const {
    if(input.flag == FRAME)
        return (input.sensor_frame == last_out_data.sensor_frame);
    return (input.seq_status == last_out_data.seq_status);
}


int link_logger::
send(const sequence_status & input) {
    ll_mutex.lock();

    // update new send data
    last_in_data.seq_status = input;
    last_in_data.flag = STATUS; 

    // send data
    send_q.enqueue(last_in_data); 

    ll_mutex.unlock();
    return 1;
}


int link_logger::
send(const sensor_data_frame & input) {
    ll_mutex.lock();

    // make data to add to queue
    last_in_data.sensor_frame = input;
    last_in_data.flag = FRAME; 

    // send data
    send_q.enqueue(last_in_data); 

    ll_mutex.unlock();
    return 1;
}


// save string
int link_logger::
save(std::string & input_string) const {
    std::ofstream file; // file for saving data on rasp pi
    file.open(FILENAME, std::fstream::app); //append to file
    file << input_string;
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


void link_logger::
make_send_string(send_data & in, std::string & out) {
    if(in.flag == FRAME)
        in.sensor_frame.make_JSON_diff(out, last_out_data.sensor_frame);
    in.seq_status.make_JSON_diff(out, last_out_data.seq_status);
    return;
}

