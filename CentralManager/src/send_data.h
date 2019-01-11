#ifndef _SEND_DATA_FRAME_H_
#define _SEND_DATA_FRAME_H_

#include "sensor_data_frame.h"
#include "sequence_status.h"

#define STATUS 1
#define FRAME 0

struct send_data {
    struct sensor_data_frame sensor_frame;
    struct sequence_status seq_status;
    bool flag; // 0 for sensor, 1 for sequence
};

#endif
