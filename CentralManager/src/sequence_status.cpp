#include "sequence_status.h"


bool operator ==  (const sequence_status & A, const sequence_status & B) {
    if(A.current_state != B.current_state)
        return 0;
    return 1;
}


bool operator !=  (const sequence_status & A, const sequence_status & B) {
    if(A.current_state == B.current_state)
        return 0;
    return 1;
}



// Uses RapidJSON to converts sequence_status struct to a vector. 
void sequence_status::
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
    new_json.AddMember("current_state", current_state, allocator);

    // converts json document to string
    StringBuffer str_buff;
    Writer<StringBuffer> writer(str_buff);
    new_json.Accept(writer);

    output = str_buff.GetString();
    return;
}


/* Uses RapidJSON to converts sequence_status struct to a vector. 
 * Only adds data that has changed to reduce data sent to clients
 */
void sequence_status::
make_JSON_diff(std::string & output, const sequence_status & other) {
    using namespace rapidjson;

    Document new_json;

    // makes json object not a string
    new_json.SetObject();

    // make allocator
    Document::AllocatorType& allocator = new_json.GetAllocator();

    // time
    new_json.AddMember("time_ms", time, allocator);

    // sensor values, only change these by adding or removing members
    if(current_state != other.current_state)
        new_json.AddMember("current_state", current_state, allocator);

    // converts json document to string
    StringBuffer str_buff;
    Writer<StringBuffer> writer(str_buff);
    new_json.Accept(writer);

    output =  str_buff.GetString();

    return;
}
