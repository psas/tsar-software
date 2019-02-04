#include "sensor_data_frame.h"

bool operator ==  (const sensor_data_frame & A, const sensor_data_frame & B){
    if(A.temp_1 != B.temp_1)
        return 0;
    if(A.pres_1 != B.pres_1)
        return 0;
    if(A.random_int != B.random_int)
        return 0;
    if(A.random_float != B.random_float)
        return 0;
    if(A.light_status != B.light_status)
        return 0;
    return 1;
}

bool operator !=  (const sensor_data_frame & A, const sensor_data_frame & B){
    if(A.temp_1 == B.temp_1)
        return 0;
    if(A.pres_1 == B.pres_1)
        return 0;
    if(A.random_int == B.random_int)
        return 0;
    if(A.random_float == B.random_float)
        return 0;
    if(A.light_status == B.light_status)
        return 0;
    return 1;
}


// Uses RapidJSON to converts sensor_data_frame struct to a vector. 
int sensor_data_frame::
make_JSON(std::string & output) {
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
    new_json.AddMember("light_status", light_status, allocator);

    // converts json document to string
    StringBuffer str_buff;
    Writer<StringBuffer> writer(str_buff);
    new_json.Accept(writer);

    output = str_buff.GetString();
    return 1;
}


/* Uses RapidJSON to converts sensor_data_frame struct to a vector. 
 * Only adds data that has changed to reduce data sent to clients
 */
int sensor_data_frame::
make_JSON_diff(const sensor_data_frame & other, std::string & output) {
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
    if(light_status != other.light_status)
    new_json.AddMember("light_status", ligth_status, allocator);

    // converts json document to string
    StringBuffer str_buff;
    Writer<StringBuffer> writer(str_buff);
    new_json.Accept(writer);

    output = str_buff.GetString();
    return 1;
}
