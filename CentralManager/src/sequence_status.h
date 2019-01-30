#ifndef _SEQ_STATUS_H_
#define _SEQ_STATUS_H_

#include <vector>

#include "../thirdparty/rapidjson/document.h"       // rapidjson
#include "../thirdparty/rapidjson/writer.h"         // rapidjson
#include "../thirdparty/rapidjson/stringbuffer.h"   // rapidjson

class sequence_status {
    public:
        int make_JSON(std::vector<char> & output);
        int make_JSON_diff(const sensor_data_frame & other, std::vector<char> & output);

        int current_state;
};

#endif
