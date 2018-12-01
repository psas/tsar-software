#ifndef _SENSOR_DATA_FRAME_H_
#define _SENSOR_DATA_FRAME_H_

#ifdef LIVE_DATA
struct sensor_data_frame {
    int time;
    int adc0;
};
#else
struct sensor_data_frame {
    int time;
    int test_int_0;
    int test_int_1;
    float test_float_0;
    float test_float_1;
};
#endif // LIVE_DATA

#endif // _SENSOR_DATA_FRAME_H_
