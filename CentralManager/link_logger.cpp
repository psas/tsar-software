#include "link_logger.h"


// default constructor
link_logger::
link_logger() : serv_thread(NULL), driver_running(0) {
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
        send_q.dequeue(temp_send_data); 
        if(data_changed(temp_send_data)) { // if data is different than last
            make_send_string(temp_send_data, output_string);
            serv.send_string(output_string);
            save(output_string);
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
    driver_running = 0;
    serv.kill_driver();
}


int link_logger::
data_changed(send_data input) const {
    if(input.flag == FRAME)
        return memcmp(&input.sensor_frame, &last_frame.sensor_frame, sensor_frame_size);
    return memcmp(&input.seq_status, &last_frame.seq_status, seq_status_size);
}


/* Send wrapper, to convert structs to string for server class to send.
 * Also saves data.
 */
int link_logger::
send(const sequence_status & input) {
    // make new send data
    send_data new_data;
    memcpy(&new_data.seq_status, &input, seq_status_size);
    new_data.flag = STATUS;

    // send data
    send_q.enqueue(new_data); 

    return 1;
}


int link_logger::
send(const sensor_data_frame & input) {
    // make data to add to queue
    send_data new_data;
    memcpy(&new_data.sensor_frame, &input, sensor_frame_size);
    new_data.flag = FRAME; 

    // send data
    send_q.enqueue(new_data); 
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


// convert recieved string to json doc to command struct
int link_logger::
make_command_data(const char * output_string, client_command & new_command) const {

    if(output_string == NULL)
        return 0;

    using namespace rapidjson;

    Document document;
    document.Parse(output_string);

    // get string
    if(document.HasMember("string") && document["string"].IsString())
        strncpy(new_command.string, document["string"].GetString(), 
                strlen(document["string"].GetString()));
    else
        return -1;

    // get int
    if(document["number"].IsNumber())
        new_command.number = document["number"].GetInt();
    else
        return -2;

    return 1;
}


/****************************************************************************/
/****** IF ADDING SENSOR(S), ONLY MODDIFY THE FUNCTION(S) BELOW THIS ********/
/****************************************************************************/


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


/* Uses RapidJSON to converts sequence_status struct to a string. 
 * Only adds data that has changed to reduce data sent to clients
 */
int link_logger::
make_send_string(const sequence_status & in_status, char * output_string) {
    using namespace rapidjson;

    Document new_json;

    // makes json object not a string
    new_json.SetObject();

    // make allocator
    Document::AllocatorType& allocator = new_json.GetAllocator();

    // sequencer status
    if(in_status.current_state != last_frame.seq_status.current_state)
        new_json.AddMember("sequence", in_status.current_state, allocator);

    // converts json document to string
    StringBuffer str_buff;
    Writer<StringBuffer> writer(str_buff);
    new_json.Accept(writer);

    // add \n\0 chars to string
    int len = strlen(str_buff.GetString())+1;
    if (len >= (RECV_STRING_MAX_LEN-2)) { // json string is too long, truncated
        std::cout << "Receive data is truncated.\n";
        strncpy(output_string, str_buff.GetString(), RECV_STRING_MAX_LEN-1);
        output_string[RECV_STRING_Q_MAX_LEN-2] = '\n';
        output_string[RECV_STRING_Q_MAX_LEN-1] = '\0';
    }
    else {
        strncpy(output_string, str_buff.GetString(), len);
        output_string[len] = '\n';
        output_string[len+1] = '\0';
    }
    
    //update last_frame for next make_send_string
    memcpy(&last_frame.seq_status, &in_status, seq_status_size);

    return 1;
}


/* Uses RapidJSON to converts sensor_data_frame struct to a string. 
 * Only adds data that has changed to reduce data sent to clients
 */
int link_logger::
make_send_string(const sensor_data_frame & in_frame, char * output_string) {
    using namespace rapidjson;

    Document new_json;

    // makes json object not a string
    new_json.SetObject();

    // make allocator
    Document::AllocatorType& allocator = new_json.GetAllocator();

    // time
    new_json.AddMember("time_ms", in_frame.time, allocator);

    // sensor values, only change these by adding or removing members
    if(in_frame.test_int_0 != last_frame.sensor_frame.test_int_0)
        new_json.AddMember("test_int_0", in_frame.test_int_0, allocator);
    if(in_frame.test_int_1 != last_frame.sensor_frame.test_int_1)
        new_json.AddMember("test_int_1", in_frame.test_int_1, allocator);
    if(in_frame.test_float_0 != last_frame.sensor_frame.test_float_0)
        new_json.AddMember("test_float_0", in_frame.test_float_0, allocator);
    if(in_frame.test_float_1 != last_frame.sensor_frame.test_float_1)
        new_json.AddMember("test_float_1", in_frame.test_float_1, allocator);

    // converts json document to string
    StringBuffer str_buff;
    Writer<StringBuffer> writer(str_buff);
    new_json.Accept(writer);

    // add \n\0 chars to string
    int len = strlen(str_buff.GetString())+1;
    if (len >= (RECV_STRING_MAX_LEN-2)) { // json string is too long, truncated
        std::cout << "Receive data is truncated.\n";
        strncpy(output_string, str_buff.GetString(), RECV_STRING_MAX_LEN-1);
        output_string[RECV_STRING_Q_MAX_LEN-2] = '\n';
        output_string[RECV_STRING_Q_MAX_LEN-1] = '\0';
    }
    else {
        strncpy(output_string, str_buff.GetString(), len);
        output_string[len] = '\n';
        output_string[len+1] = '\0';
    }

    //update last_frame for next make_send_string
    memcpy(&last_frame.sensor_frame, &in_frame, sensor_frame_size);

    return 1;
}
