#ifndef _SENSOR_DATA_FRAME_H_
#define _SENSOR_DATA_FRAME_H_

#include <string>

#include "../thirdparty/rapidjson/document.h"       // rapidjson
#include "../thirdparty/rapidjson/writer.h"         // rapidjson
#include "../thirdparty/rapidjson/stringbuffer.h"   // rapidjson

class sensor_data_frame {
    public:
        friend bool operator ==  (const sensor_data_frame &, const sensor_data_frame &);
        friend bool operator !=  (const sensor_data_frame &, const sensor_data_frame &);

        int make_JSON(std::string & output);
        int make_JSON_diff(const sensor_data_frame & other, std::string & output);

        int time;

        // i2c 
        float temp_1;
        float pres_1;
        int random_int;
        float random_float;

        // gpio
        int light_status;
        // UADT ?
};

#endif // _SENSOR_DATA_FRAME_H_
