#ifndef _SENSOR_DATA_FRAME_H_
#define _SENSOR_DATA_FRAME_H_

#include <vector>

#include "../thirdparty/rapidjson/document.h"       // rapidjson
#include "../thirdparty/rapidjson/writer.h"         // rapidjson
#include "../thirdparty/rapidjson/stringbuffer.h"   // rapidjson

class sensor_data_frame {
    public:
        int make_JSON(std::vector<char> & output);
        int make_JSON_diff(const sensor_data_frame & other, std::vector<char> & output);

        int time;
        int temp_1;
        int pres_1;
        int random_int;
        float random_float;
};

#endif // _SENSOR_DATA_FRAME_H_
