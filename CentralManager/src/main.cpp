#include <iostream>         // cout
#include <thread>           // thread
#include <unistd.h>         // getuid

#include "sequencer.h"
#include "link_logger.h"
#include "hardware_controller.h"

#define HDW_PRIO 20         // hardware thread priority
#define SEQ_HIGH_PRIO 30    // sequencer high thread priority
#define SEQ_LOW_PRIO 10     // sequencer low thread priority
#define LINK_PRIO 5         // link logger thread priority

int check_sudo();

int
main() {
    if(check_sudo() == 0)
        return 0;

    // make main objects
    std::shared_ptr<link_logger> link;
    std::shared_ptr<hardware_controller> hdw(link);
    std::unique_ptr<sequencer> seq(link, hdw);

    std::cout << "Hardware Controller thread started" << std::endl;
    std::thread hdw_thread(&hardware_controller::driver_loop, hdw);
    //pthread_setschedparam(hdw_thread.native_handle(), SCHED_RR, HDW_PRIO);

    std::cout << "Link Logger thread started" << std::endl;
    std::thread link_thread(&link_logger::driver_loop, link);
    //pthread_setschedparam(link_thread.native_handle(), SCHED_RR, LINK_PRIO);

    std::cout << "Sequencer thread started" << std::endl;
    std::thread seq_thread(&sequencer::driver_loop_main, &seq);
    // pthread_setschedparam(seq_thread.native_handle(), SCHED_RR, SEQ_MAIN_PRIO);

    int sytem_on = 1;
    seq->driver_loop_high();

    link->kill_driver();
    hdw->kill_driver();
    seq->kill_driver();

    sleep(5); // give time for threads to shutdown

    link_thread.join();
    hdw_thread.join();
    seq_thread.join();

    return 1;
}

int
check_sudo() {
    if (getuid()) {
        std::cout << "Run as root." << std::endl;
        return 0;
    }
    else
        return 1;
}

