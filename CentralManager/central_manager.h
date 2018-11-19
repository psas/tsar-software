#ifndef _CM_H_
#define _CM_H_

#include <thread> // thread
#include <unistd.h> // getuid
#include <iostream> // cout

#include "sequencer.h"
#include "link_logger.h"
#include "hardware_controller.h"

#define HDW_PRIO 20 
#define SEQ_HIGH_PRIO 30
#define SEQ_LOW_PRIO 10
#define LINK_PRIO 5

/* Governor:
 * Its main job is build and control all threads aswell as getting the 
 * sensor_data_frame and sequence_status to pass to the link_logger to 
 * send data to clients.
 */
class central_manager {
    public:
        central_manager();
        ~central_manager();

        void start_system();
    private:
        void start_threads();
        void kill_all_threads();
        int check_sudo();

        // ---------- data -------------
        std::thread * hdw_thread;
        std::thread * link_thread;
        std::thread * seq_thread;

        link_logger link;
        hardware_controller hdw;
        sequencer seq;
};

#endif
