#include "central_manager.h"


central_manager::
~central_manager() {
    kill_all_threads(); // make sure all thread are stoped

    if(link_thread != NULL){
        delete link_thread;
        link_thread = NULL;
    }
    if(hdw_thread != NULL){
        delete hdw_thread;
        hdw_thread = NULL;
    }
    if(seq_thread != NULL){
        delete seq_thread;
        seq_thread = NULL;
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
    hdw_thread = new std::thread(&hardware_controller::driver_loop, &hdw);
    //pthread_setschedparam(hdw_thread.native_handle(), SCHED_RR, HDW_PRIO);

    std::cout << "Link Logger thread started" << endl;
    link_thread = new std::thread(&link_logger::driver_loop, &link);
    //pthread_setschedparam(link_thread.native_handle(), SCHED_RR, LINK_PRIO);

    std::cout << "Sequencer thread started" << endl;
    seq_thread = new std::thread(&sequencer::driver_loop_main, &seq);
   // pthread_setschedparam(seq_thread.native_handle(), SCHED_RR, SEQ_MAIN_PRIO);
}

void central_manager::
kill_all_threads() {
    link.kill_driver();
    hdw.kill_driver();
    seq.kill_driver();

    link_thread->join();
    hdw_thread->join();
    seq_thread->join();
}

// check for root, thread priority can't used without root
int central_manager::
check_sudo() {
    if (getuid()) {
        std::cout << "Run as root.\n";
        return 0;
    }
    else
        return 1;
}
