#ifndef _SEQUENCER_H_
#define _SEQUENCER_H_

#include <mutex>                // mutex
#include <memory>               // sharred_ptr
#include <atomic>               // atomic
#include <chrono>               // delays
#include <thread>               // sleep_for
#include <iostream>

#include "main_class.h"
#include "sequencer_status.h"
#include "hardware/hardware_controller.h"
#include "hardware_data_frame.h"
#include "link_logger.h"
#include "client_command.h"

#define SEQ_DRIVER_DELAY 500    //microseconds

/* Sequencer:
 * Control theory.
 */
class sequencer : public main_class {
    public:
        sequencer(std::shared_ptr<link_logger> & link_input, std::shared_ptr<hardware_controller> & hdw_ctrl_input);

        void driver_loop();
        bool is_running();
    private:
        bool emergency_state();
        int sequence();

        std::shared_ptr<link_logger> link;
        std::shared_ptr<hardware_controller> hdw_ctrl;

        hardware_data_frame last_hdw_frame;
        sequencer_status status;

        std::chrono::system_clock::time_point wait_until_time;

        std::atomic<bool> driver_running;
};

#endif
