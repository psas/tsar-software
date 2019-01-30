#include "link_logger.h"


// default constructor
link_logger::
link_logger() : serv_thread(NULL), send_data(SEND_DATA_Q_LEN), driver_running(0) {
    send_data_size = sizeof(struct send_data);
    sensor_frame_size = sizeof(struct sensor_data_frame);
    seq_status_size = sizeof(struct sequence_status);

    driver_delay.tv_sec = 0;
    driver_delay.tv_nsec = LINK_LOGGER_DELAY * 1000000;  
}


// deconstructor
link_logger::
~link_logger() {
    serv.kill_driver();
    if(serv_thread != NULL) {
        serv_thread->join();
        delete serv_thread;
        serv_thread = NULL;
    }
}


// server wrapper for link thread
void link_logger::
driver_loop() {
    start_server();
    char output_string[SEND_STRING_MAX_LEN];
    send_data temp_send_data;

    std::cout << "Link Logger Started\n";

    driver_running = 1;
    while(driver_running) {
        while(send_q.dequeue(temp_send_data) == 1) { 
            // TODO check if possible to combine with if statement and the compiler kept correct the order
            if(data_changed(temp_send_data) != 0) { // making sure dequeue happens 1st
                // if the temp data is differenet than last 
                make_send_string(temp_send_data, output_string);
                serv.send_string(output_string);
                save(output_string);
            }
        }
        nanosleep(&driver_delay, NULL);
    }

    std::cout << "Link Logger Stopped\n";
}


void link_logger::
start_server() {
    serv_thread = new std::thread(&server::driver_loop, &serv);
}


// kill server driver wrapper
void link_logger::
kill_driver() { 
    ll_mutex.lock();
    driver_running = 0;
    serv.kill_driver();
    ll_mutex.unlock();
}


int link_logger::
data_changed(send_data input) const {
    if(input.flag == FRAME)
        return memcmp(&input.sensor_frame, &last_frame.sensor_frame, sensor_frame_size);
    return memcmp(&input.seq_status, &last_frame.seq_status, seq_status_size);
}


int link_logger::
send(const sequence_status & input) {
    ll_mutex.lock();
    // update new send data
    new_data.seq_status = input;
    new_data.flag = STATUS; 

    // send data
    send_q.enqueue(new_data); 
    ll_mutex.unlock();
    return 1;
}


int link_logger::
send(const sensor_data_frame & input) {
    ll_mutex.lock();
    // make data to add to queue
    new_data.sensor_frame = input;
    new_data.flag = FRAME; 

    // send data
    send_q.enqueue(new_data); 
    ll_mutex.unlock();
    return 1;
}


// save string
int link_logger::
save(const char * input_string) const {
    std::ofstream file; // file for saving data on rasp pi
    file.open(FILENAME, std::fstream::app); //append to file
    file << input_string;
    file.close();
    return 1;
}


// Receive wrapper, to convert string from server to a command struct for processing 
int link_logger::
recv(client_command & output) {
    char output_string[RECV_STRING_MAX_LEN];

    if(serv.recv_string(output_string) == 0) // no new string
        return 0;
    make_command_data(output_string, output);
    return 1;
}


/* Wrapper function for converting sensor_data_frame and sequence_status structs
 * to a string using RAPIDJSON. Only adds data that has changed to reduce data
 * sent to clients
 */
int link_logger::
make_send_string(const send_data & input_data, char * output_string) {
    int ri;
    if(input_data.flag == STATUS)
        ri = make_send_string(input_data.seq_status, output_string);
    else // == FRAME
        ri = make_send_string(input_data.sensor_frame, output_string);
    return ri;
}

