#include "sensor_data_frame.h"

bool operator ==  (const sensor_data_frame & A, const sensor_data_frame & B){
    if(A.temp_1 != B.temp_1)
        return 0;
    if(A.pres_1 != B.pres_1)
        return 0;
    if(A.temp_2 != B.temp_2)
        return 0;
    if(A.pres_2 != B.pres_2)
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
    if(A.temp_2 == B.temp_2)
        return 0;
    if(A.pres_2 == B.pres_2)
        return 0;
    if(A.light_status == B.light_status)
        return 0;
    return 1;
}


// Uses RapidJSON to converts sensor_data_frame struct to a vector. 
void sensor_data_frame::
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
    new_json.AddMember("temp_2", temp_2, allocator);
    new_json.AddMember("pres_2", pres_2, allocator);
    new_json.AddMember("light_status", light_status, allocator);

    // converts json document to string
    StringBuffer str_buff;
    Writer<StringBuffer> writer(str_buff);
    new_json.Accept(writer);

    output = str_buff.GetString();
    return;
}


/* Uses RapidJSON to converts sensor_data_frame struct to a vector. 
 * Only adds data that has changed to reduce data sent to clients
 */
void sensor_data_frame::
make_JSON_diff(std::string & output, const sensor_data_frame & other) {
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
    if(temp_2 != other.temp_2)
        new_json.AddMember("temp_2", temp_2, allocator);
    if(pres_2 != other.pres_2)
        new_json.AddMember("pres_2", pres_2, allocator);
    if(light_status != other.light_status)
        new_json.AddMember("light_status", light_status, allocator);

    // converts json document to string
    StringBuffer str_buff;
    Writer<StringBuffer> writer(str_buff);
    new_json.Accept(writer);

    output = str_buff.GetString();
    return;
}
