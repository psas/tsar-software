#ifndef __CC__
#define __CC__

#include <thread>               // sleep_for
#include <chrono>               // time
#include <memory>               // shared ptr
#include <iostream>             //printing stuff
#include <ncurses.h>
#include <cstring>

#include "central_manager.h"
#include "state.h"

#define CC_DELAY 100            // milliseconds

#define CMD_BUF_SZ          21
#define EMERGENCY_KEY       '-'

#define HEADER_ROW          0
    // line 1 is empty
#define STATE_ROW_0         2
#define STATE_ROW_1         3
#define STATE_ROW_2         4
#define STATE_ROW_3         5
#define STATE_ROW_4         6
#define STATE_ROW_5         7
#define STATE_ROW_6         8
#define STATE_ROW_7         9
#define STATE_ROW_8         10
#define STATE_ROW_9         11
#define STATE_ROW_10        12   //TODO ADD APC
#define STATE_ROW_11        13
    // line 14 is empty
#define DATA_ROW_0          15
#define DATA_ROW_1          16
#define DATA_ROW_2          17
#define DATA_ROW_3          18
#define DATA_ROW_4          19
#define DATA_ROW_5          20
    // line 21 is empty
#define SAVE_FILE_ROW       22
    // line 23 is empty
#define LAST_COMMAND_ROW    24
#define INPUT_ROW           25

#define STATE_LABEL_LENGTH  5




// This class will start the system, make both threads, 
// and handle input from the user.
class CommandControl{
    public:
        CommandControl();
        int start_system();

    private:
        int interface();
        void start_CM_thread();
        int char_aval();
        int print_labels();
        int read_input();
        int print_data();
        int print_state_info(bool & valve_state, int print_row);

        char input_buf[CMD_BUF_SZ]; 
        std::string command; 
        int i;
        int row, col;
        std::shared_ptr<CentralManager> CM;
        std::thread CM_thread;
        State state;
};

#endif  // __CC__
