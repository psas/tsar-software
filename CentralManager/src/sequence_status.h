#ifndef _SEQ_STATUS_H_
#define _SEQ_STATUS_H_

#include <string>

#include "../thirdparty/rapidjson/document.h"       // rapidjson
#include "../thirdparty/rapidjson/writer.h"         // rapidjson
#include "../thirdparty/rapidjson/stringbuffer.h"   // rapidjson

/* This a POD (Plain Old Data) struct used for to hold the sequencer status infomation.
 * It tehcically is not a POD, but the function only converts the internal data into a 
 * JSON string, so for the most part it is.
 */
struct sequence_status {
        friend bool operator ==  (const sequence_status &, const sequence_status &);
        friend bool operator !=  (const sequence_status &, const sequence_status &);
        void make_JSON(std::string & output);
        void make_JSON_diff(std::string & output, const sequence_status & input);

        int current_state;
};

#endif
