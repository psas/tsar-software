#include <iostream>         // cout
#include <thread>           // thread
#include <chrono>           // sleep for
#include <atomic>           // atomic

#include "sequencer.h"
#include "hardware_controller.h"
#include "link_logger.h"
#include "server.h"

#define GOV_DRIVER_DELAY 500 // mircoseconds


/* This class will handle the setup/end the entire Central Manager system and will 
 * control all threads needed. If any thread ends early, it will restart that thread. 
 * Once the Sequencer has declared it has finish sequencing, the Governor will end all 
 * threads and exit.
 */
class governor {
    public:
        governor();
        ~governor();

        /* Calls all run function below to (re)start threads as needed, 
         * until sequencer is done sequencing.
         */
        void start_system();

    private:
        // These functions start the system threads, if they return, the thread has ended
        void run_hdw();
        void run_seq();
        void run_link();
        void run_server();

        // threads
        std::thread hdw_thread;
        std::thread seq_thread;
        std::thread link_thread;
        std::thread server_thread;

        // use to know the status of sub threads
        std::atomic<bool> seq_thread_running;
        std::atomic<bool> hdw_thread_running;
        std::atomic<bool> link_thread_running;
        std::atomic<bool> server_thread_running;

        // pointer to all sub-classes
        std::unique_ptr<sequencer> seq;
        std::shared_ptr<hardware_controller> hdw_ctrl;
        std::shared_ptr<link_logger> link;
        std::shared_ptr<server> serv;

        std::mutex gov_mutex;
};

