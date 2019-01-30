#include "sensor_data_frame.h"


// Uses RapidJSON to converts sensor_data_frame struct to a vector. 
int sensor_data_frame::
make_JSON(std::vector<char> & output) {
    using namespace rapidjson;

    Document new_json;

    // makes json object not a string
    new_json.SetObject();

    // make allocator
    Document::AllocatorType& allocator = new_json.GetAllocator();

    // time
    new_json.AddMember("time_ms", time, allocator);

    // sensor values, only change these by adding or removing members
    new_json.AddMember("temp_1", temp_1, allocator);
    new_json.AddMember("pres_1", pres_1, allocator);
    new_json.AddMember("random_int", random_int, allocator);
    new_json.AddMember("random_float", random_float, allocator);

    // converts json document to string
    StringBuffer str_buff;
    Writer<StringBuffer> writer(str_buff);
    new_json.Accept(writer);

    std::string temp = str_buff.GetString();

    // resize vector if needed
    unsigned int len = temp.size();
    if(output.size() < len+2) {
        output.resize(len+2);
    }

    // copy into output vector
    for(unsigned int i=0; i<temp.size(); ++i)
        output[i] = temp[i];

    // add \n\0 chars to vector
    output[len] = '\n';
    output[len+1] = '\0';

    return 1;
}


/* Uses RapidJSON to converts sensor_data_frame struct to a vector. 
 * Only adds data that has changed to reduce data sent to clients
 */
int sensor_data_frame::
make_JSON_diff(const sensor_data_frame & other, std::vector<char> & output) {
    using namespace rapidjson;

    Document new_json;

    // makes json object not a string
    new_json.SetObject();

    // make allocator
    Document::AllocatorType& allocator = new_json.GetAllocator();

    // time
    new_json.AddMember("time_ms", time, allocator);

    // sensor values, only change these by adding or removing members
    if(temp_1 != other.temp_1)
        new_json.AddMember("temp_1", temp_1, allocator);
    if(pres_1 != other.pres_1)
        new_json.AddMember("pres_1", pres_1, allocator);
    if(random_int != other.random_int)
        new_json.AddMember("random_int", random_int, allocator);
    if(random_float != other.random_float)
        new_json.AddMember("random_float", random_float, allocator);

    // converts json document to string
    StringBuffer str_buff;
    Writer<StringBuffer> writer(str_buff);
    new_json.Accept(writer);

    std::string temp = str_buff.GetString();

    // resize vector if needed
    unsigned int len = temp.size();
    if(output.size() < len+2) {
        output.resize(len+2);
    }

    // copy into output vector
    for(unsigned int i=0; i<temp.size(); ++i)
        output[i] = temp[i];

    // add \n\0 chars to vector
    output[len] = '\n';
    output[len+1] = '\0';

    return 1;
}
