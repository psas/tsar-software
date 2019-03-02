#ifndef _AC_DATA_FRAME_H_
#define _AC_DATA_FRAME_H_

#include <cstdint>  // uintx_t


/* This a POD (Plain Old Data) struct used for to hold a snap shot of the data
 * received from the Actuactor Controller.
 */
struct AC_data_frame {
    uint8_t next_failure_mode;
    uint8_t failure_mode;
    uint8_t failure_cause;
    uint8_t consecutive_checksum_errors;
    uint8_t lifetime_checksum_errors;
    uint8_t sensor1;
    uint8_t sensor2;
    uint8_t sensor3;
    uint8_t sensor4;
    uint16_t sensor5;
};

#endif // _AC_DATA_FRAME_H_
