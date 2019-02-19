#ifndef _SENSOR_DATA_FRAME_H_
#define _SENSOR_DATA_FRAME_H_

#include <string>   // string
#include <cstdint>  // uintx_t

#include "../thirdparty/rapidjson/writer.h"         // rapidjson
#include "../thirdparty/rapidjson/stringbuffer.h"   // rapidjson

/* This a POD (Plain Old Data) struct used for to hold a snap shot of the sensor data 
 * and hardware controller status infomation. It tehcically is not a POD, but the function only 
 * converts the internal data into a JSON string, so for the most part it is.
 */
class sensor_data_frame {
    public:
        // these are overload to ignore the time field, since every object will have a different time value
        friend bool operator == (const sensor_data_frame & A, const sensor_data_frame & B);
        friend bool operator != (const sensor_data_frame & A, const sensor_data_frame & B);

        void make_JSON(std::string & output);
        void make_JSON_diff(std::string & output, const sensor_data_frame & other);

        std::string time;

        // i2c 
        double temp_1;
        double pres_1;
        double temp_2;
        double pres_2;

        // gpio
        bool light_status;

        // Actuator Controller Valuse
        bool AC_connected;
        uint8_t AC_next_failure_mode;
        uint8_t AC_failure_mode;
        uint8_t AC_failure_cause;
        uint8_t AC_consecutive_checksum_errors;
        uint8_t AC_lifetime_checksum_errors;
        uint8_t AC_sensor1;
        uint8_t AC_sensor2;
        uint8_t AC_sensor3;
        uint8_t AC_sensor4;
        uint16_t AC_sensor5;
};

#endif // _SENSOR_DATA_FRAME_H_
