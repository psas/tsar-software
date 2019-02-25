#ifndef _I2C_DATA_FRAME_H_
#define _I2C_DATA_FRAME_H_

#include <cstdint>  // uintx_t

/* This a POD (Plain Old Data) struct used for to hold a snap shot of all the 
 * data of the i2c sensors and their status infomation.
 */
struct i2c_data_frame {
    bool sensor_1_connected;
    double temp_1;
    double pres_1;
    
    bool sensor_2_connected;
    double temp_2;
    double pres_2;
};

#endif // _I2C_DATA_FRAME_H_
