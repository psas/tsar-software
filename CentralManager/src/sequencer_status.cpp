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


// TODO change to constexpr?
std::string sequencer_status::
enum_to_string(const int & in) {
    if (in == eEnd) return "end";
    else if (in == eStart) return std::string("start");
    else if (in == eHalt) return std::string("halt");
    else if (in == eWait) return std::string("walt");
    else if (in == eLightOff) return std::string("light_off");
    else if (in == eLightOn) return std::string("light_on");
    else return std::string("unknown");
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
    writer.String(enum_to_string(current_state).c_str());

    writer.Key("next_state");
    writer.String(enum_to_string(next_state).c_str());

    if(current_state == eHalt) {
        writer.Key("state_after_halt");
        writer.String(enum_to_string(state_after_halt).c_str());
    }
    else if(current_state == eWait) {
        writer.Key("state_after_wait");
        writer.String(enum_to_string(state_after_wait).c_str());
    }

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
        writer.String(enum_to_string(current_state).c_str());
    }

    if(next_state != other.next_state) {
        writer.Key("next_state");
        writer.String(enum_to_string(next_state).c_str());
    }

    if(current_state == eHalt && current_state == other.current_state) {
        writer.Key("state_after_halt");
        writer.String(enum_to_string(state_after_halt).c_str());
    }
    else if(current_state == eWait && next_state == other.next_state) {
        writer.Key("state_after_wait");
        writer.String(enum_to_string(state_after_wait).c_str());
    }

    writer.EndObject();
    output = s.GetString();

    return;
}
