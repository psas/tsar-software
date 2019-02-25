#include "sequencer_status.h"


bool operator ==  (const sequencer_status & A, const sequencer_status & B) {
    if(A.current_state != B.current_state)
        return 0;
    return 1;
}


bool operator !=  (const sequencer_status & A, const sequencer_status & B) {
    if(A.current_state == B.current_state)
        return 0;
    return 1;
}



// Uses RapidJSON to converts sequencer_status struct to a vector. 
void sequencer_status::
make_JSON(std::string & output) {
    rapidjson::StringBuffer s;
    rapidjson::Writer<rapidjson::StringBuffer> writer(s);
    //writer.SetMaxDecimalPlaces(6);

    writer.StartObject();

    writer.Key("what");
    writer.String("seq_full");

    // time
    writer.Key("time");
    writer.String(time.c_str());

    writer.Key("current_state");
    writer.Uint(current_state);

    writer.EndObject();
    output = s.GetString();

    return;
}


/* Uses RapidJSON to converts sequencer_status struct to a vector. 
 * Only adds data that has changed to reduce data sent to clients
 */
void sequencer_status::
make_JSON_diff(std::string & output, const sequencer_status & other) {
    rapidjson::StringBuffer s;
    rapidjson::Writer<rapidjson::StringBuffer> writer(s);
    //writer.SetMaxDecimalPlaces(6);

    writer.StartObject();

    writer.Key("what");
    writer.String("seq_change");

    // time
    writer.Key("time");
    writer.String(time.c_str());

    if(current_state != other.current_state) {
        writer.Key("current_state");
        writer.Uint(current_state);
    }

    writer.EndObject();
    output = s.GetString();

    return;
}
