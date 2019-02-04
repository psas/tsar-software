#include "link_logger.h"


// default constructor
link_logger::
link_logger() : serv_thread(NULL), send_q(SEND_DATA_Q_LEN), recv_q(CLIENT_COM_Q_LEN), driver_running(0) {
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

    std::string output_string(SEND_STRING_MAX_LEN, '\0');
    send_data temp_send_data;

    std::cout << "Link Logger Started\n";

    driver_running = 1;
    while(driver_running) {
        while(send_q.dequeue(temp_send_data) && data_changed(temp_send_data)) {
                // if there is temp data and it's differenet than last 
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
        return (input.sensor_frame == last_frame.semsor_frame);
    return (input.seq_status == last_frame.seq_satus);
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

    if(serv.recv_string(temp_string) == 0) // no new string
        return 0;

    ouput.make_command_data(temp_string);
    return 1;
}


/* Wrapper function for converting sensor_data_frame and sequence_status structs
 * to a string using RAPIDJSON. Only adds data that has changed to reduce data
 * sent to clients
 */
int link_logger::
make_send_string(const send_data & input_data, std::string & output_string) {
    int ri;

    if(input_data.flag == STATUS)
        ri = input_data.seq_status.make_JSON(output_string);
    else // == FRAME
        ri = make_send_string(input_data.sensor_frame, output_string);

    return ri;
}

