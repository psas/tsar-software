#ifndef _GPIO_DATA_FRAME_H_
#define _GPIO_DATA_FRAME_H_

#define GPIO_DISCONNECTED   -1

/* This a POD (Plain Old Data) struct used for to hold a snap shot of the status of 
 * all gpio pins.
 */
struct gpio_data_frame {
    int light_1_status;
    int light_2_status;
};

#endif // _GPIO_DATA_FRAME_H_
