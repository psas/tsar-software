#ifndef _SENSOR_DATA_FRAME_H_
#define _SENSOR_DATA_FRAME_H_

#include <string>   // string
#include <cstdint>  // uintx_t

#include "writer.h"         // rapidjson
#include "stringbuffer.h"   // rapidjson

/* This holds a snap shot all  sensor data and status infomation.
 * This struct has functions to convert its data in to a json string for sending to clients.
 */
struct sensor_data_frame {
    void make_JSON(std::string & output);

    // data here!!!!!!!!!!
};

#endif // _SENSOR_DATA_FRAME_H_
