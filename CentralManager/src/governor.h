#include <unistd.h>         // getuid
#include <iostream>         // cout, cerr
#include <thread>           // thread
#include <chrono>           // sleep for
#include <atomic>           // atomic
#include <mutex>            // mutex

#include "main_class.h"
#include "sequencer.h"
#include "hardware/hardware_controller.h"
#include "link_logger.h"
#include "server.h"

#define GOV_DRIVER_DELAY 500    // mircoseconds
#define NICE_VALUE -20          // nice
#define SEQ_THREAD_PRIO 10
#define HDW_THREAD_PRIO 10
#define LINK_THREAD_PRIO 10
#define SERV_THREAD_PRIO 10


/* This class will handle the setup/end the entire Central Manager system and will 
 * control all threads needed. If any thread ends early, it will restart that thread. 
 * Once the Sequencer has declared it has finish sequencing, the Governor will end all 
 * threads and exit.
 */
class governor : public main_class {
    public:
        governor();
        ~governor();

        /* Calls all run function below to (re)start threads as needed, 
         * until sequencer is done sequencing.
         */
        void driver_loop();

    private:
        // These functions start the system threads, if they return, the thread has ended
        void run_hdw();
        void run_seq();
        void run_link();
        void run_server();

        // sub threads
        std::thread hdw_thread;
        std::thread seq_thread;
        std::thread link_thread;
        std::thread server_thread;

        // used to know the status of sub threads
        std::atomic<bool> seq_thread_running;
        std::atomic<bool> hdw_thread_running;
        std::atomic<bool> link_thread_running;
        std::atomic<bool> server_thread_running;

        // pointers to all sub-classes
        std::shared_ptr<sequencer> seq;
        std::shared_ptr<hardware_controller> hdw_ctrl;
        std::shared_ptr<link_logger> link;
        std::shared_ptr<server> serv;
};

