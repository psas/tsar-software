#ifndef _SEQ_STATUS_H_
#define _SEQ_STATUS_H_

#include <string>

#include "../thirdparty/rapidjson/writer.h"         // rapidjson
#include "../thirdparty/rapidjson/stringbuffer.h"   // rapidjson


// state that sequencer can be in defined by the sequence function
// emergency states are include since
enum eStates {
    eUnknown,
    eEmergency,
    eEnd,
    eStop,
    eStart,
    eHalt,
    eWait,
    eLightOff,
    eLightOn,
};


/* This a POD (Plain Old Data) struct used for to hold the sequencer status infomation.
 * It tehcically is not a POD, but the function only converts the internal data into a 
 * JSON string, so for the most part it is.
 */
struct sequencer_status {
        friend bool operator ==  (const sequencer_status &, const sequencer_status &);
        friend bool operator !=  (const sequencer_status &, const sequencer_status &);
        std::string enum_to_string(const int & in);
        void make_JSON(std::string & output);
        void make_JSON_diff(std::string & output, const sequencer_status & input);

        std::string time;
        int current_state;
        int next_state;
        int state_after_wait; // only used for waits
        int state_after_halt; // only used for halts
        int state_after_emergency; // only used for emergencies
};

#endif
