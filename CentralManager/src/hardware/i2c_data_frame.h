#ifndef _I2C_DATA_FRAME_H_
#define _I2C_DATA_FRAME_H_

#include <cstdint>  // uintx_t

/* This a POD (Plain Old Data) struct used for to hold a snap shot of all the 
 * data of the i2c sensors and their status infomation.
 */
struct i2c_data_frame {
    bool temp_1_connected;
    double temp_1;
    
    bool pres_1_connected;
    double pres_1;
    
    bool temp_2_connected;
    double temp_2;

    bool pres_2_connected;
    double pres_2;
};

#endif // _I2C_DATA_FRAME_H_
