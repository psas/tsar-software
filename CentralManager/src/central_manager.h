#ifndef _CM_H_
#define _CM_H_

#include <thread>           // thread
#include <unistd.h>         // getuid
#include <iostream>         // cout

#include "sequencer.h"
#include "link_logger.h"
#include "hardware_controller.h"

#define HDW_PRIO 20         // hardware thread priority
#define SEQ_HIGH_PRIO 30    // sequencer high thread priority
#define SEQ_LOW_PRIO 10     // sequencer low thread priority
#define LINK_PRIO 5         // link logger thread priority

/* Central Manager:
 * Its main job is to setup the hardware, sequencer, and link logger classes and threads.
 */
class central_manager {
    public:
        central_manager();
        ~central_manager();

        void start_system();
    private:
        void start_threads();
        int check_sudo();

        // ---------- data -------------
        std::unique_ptr<std::thread> link_thread;
        std::unique_ptr<std::thread> hdw_thread;
        std::unique_ptr<std::thread> seq_thread;

        std::shared_ptr<link_logger> link;
        std::shared_ptr<hardware_controller> hdw;
        sequencer seq;
};

#endif
