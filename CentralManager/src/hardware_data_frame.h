#ifndef _SENSOR_DATA_FRAME_H_
#define _SENSOR_DATA_FRAME_H_

#include <string>   // string
#include <cstdint>  // uintx_t

#include "hardware/i2c_data_frame.h"
#include "hardware/AC_data_frame.h"
#include "../thirdparty/rapidjson/writer.h"         // rapidjson
#include "../thirdparty/rapidjson/stringbuffer.h"   // rapidjson

/* This holds a snap shot all hardware sensor data and status infomation.
 * This struct has functions to convert its data in to a json string for sending to clients.
 */
struct hardware_data_frame {
        // these are overload to ignore the time field, since every object will have a different time value
        friend bool operator == (const hardware_data_frame & A, const hardware_data_frame & B);
        friend bool operator != (const hardware_data_frame & A, const hardware_data_frame & B);

        void make_JSON(std::string & output);
        void make_JSON_diff(std::string & output, const hardware_data_frame & other);

        std::string time;

        // i2c
        i2c_data_frame i2c_data;

        // gpio
        bool light_status;

        // Actuator Controller Valuse
        bool AC_connected;
        AC_data_frame AC_data;
};

#endif // _SENSOR_DATA_FRAME_H_
