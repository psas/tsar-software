#ifndef _SEQUENCER_H_
#define _SEQUENCER_H_

#include <ctime>
#include <iostream>
#include <cstdint>

#include "sensor_data_frame.h"
#include "hardware_controller.h"
#include "link_logger.h"
#include "sequence_status.h"

#define SEQ_MAIN_DRIVER_DELAY 50 //milliseconds
#define SEQ_HIGH_DRIVER_DELAY 10 //milliseconds

/* Sequencer:
 * Control theory.
 */
class sequencer {
    public:
        sequencer(link_logger * link, hardware_controller * hdw_ctrl);
        ~sequencer();

        void driver_loop_high();
        void driver_loop_main();
        void kill_driver();
    private:
        int emergency_state();
        int sequence();

        // --------- data -----------
        link_logger * link;
        hardware_controller * hdw_ctrl;

        struct sensor_data_frame last_frame;
        struct sequence_status status;
        uint32_t status_size;

        struct timespec high_driver_delay;
        struct timespec main_driver_delay;
        uint32_t main_driver_running;
        uint32_t high_driver_running;
};
#endif
