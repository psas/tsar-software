#include "central_manager.h"

central_manager::central_manager() : link(), hdw(link), seq(link, hdw),
        link_thread(nullptr), hdw_thread(nullptr), seq_thread(nullptr) {}

central_manager::
~central_manager() {
    link.kill_driver();
    hdw.kill_driver();
    seq.kill_driver();

    link_thread->join();
    hdw_thread->join();
    seq_thread->join();

    if(link_thread != nullptr){
        delete link_thread;
        link_thread = nullptr;
    }
    if(hdw_thread != nullptr){
        delete hdw_thread;
        hdw_thread = nullptr;
    }
    if(seq_thread != nullptr){
        delete seq_thread;
        seq_thread = nullptr;
    }
}

void central_manager::
start_system() {
    if(check_sudo() == 0) // not root
        return;
    start_threads();
    seq.driver_loop_high();
}

void central_manager::
start_threads() {
    std::cout << "Hardware Controller thread started" << endl;
    hdw_thread = new std::thread(&hardware_controller::driver_loop, hdw);
    //pthread_setschedparam(hdw_thread.native_handle(), SCHED_RR, HDW_PRIO);

    std::cout << "Link Logger thread started" << endl;
    link_thread = new std::thread(&link_logger::driver_loop, link);
    //pthread_setschedparam(link_thread.native_handle(), SCHED_RR, LINK_PRIO);

    std::cout << "Sequencer thread started" << endl;
    seq_thread = new std::thread(&sequencer::driver_loop_main, &seq);
   // pthread_setschedparam(seq_thread.native_handle(), SCHED_RR, SEQ_MAIN_PRIO);
}

// check for root, thread priority can't be used without root
int central_manager::
check_sudo() {
    if (getuid()) {
        std::cout << "Run as root.\n";
        return 0;
    }
    else
        return 1;
}
