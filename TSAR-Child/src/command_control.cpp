#include "command_control.h"


CommandControl::
CommandControl() {
    CM = std::shared_ptr<CentralManager>(new CentralManager);
}


int CommandControl::
start_system(){
    std::thread CM_thread(&CommandControl::start_CM_thread , this); //Creates a thread CM_thread, and sends CM_loop as a function pointer

    initscr();  // start ncurses
    nodelay(stdscr, TRUE); // dont wait for input
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_RED);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    
    interface();
    
    endwin();   // end ncurses

    if(CM_thread.joinable())
        CM_thread.join();

    return 1;
}


void CommandControl::start_CM_thread(){
    CM->CM_loop();
    return;
}


int CommandControl::
interface() {
    int j=0; // TODO remove
    getmaxyx(stdscr, row, col);
    print_labels();

    i=0;
    input_buf[i] = '\0'; 

    while(1){ //TODO add end state or break

        read_input();
        CM->copy_data(state);
        print_data();

        mvprintw(0, 0, "%d", ++j);// TODO remove
        move(INPUT_ROW, 7+i); // kept curser on INPUT line during sleep

        refresh();  // refresh
        std::this_thread::sleep_for(std::chrono::milliseconds(CC_DELAY));
    }
    return 1;
}

int CommandControl::
print_labels() {
    char mesg[] = "TSAR-Child "; 
    mvprintw(0, (col-strlen(mesg))/2, "%s", mesg);

    mvprintw(STATE_ROW_0, 0, "Current State: ");
    mvprintw(STATE_ROW_1, 0, "APC: ");
    mvprintw(STATE_ROW_2, 0, "VVO: ");
    mvprintw(STATE_ROW_3, 0, "VVF: ");
    mvprintw(STATE_ROW_4, 0, "OPV: ");
    mvprintw(STATE_ROW_5, 0, "FPV: ");
    mvprintw(STATE_ROW_6, 0, "PPV: ");
    mvprintw(STATE_ROW_7, 0, "IV1: ");
    mvprintw(STATE_ROW_8, 0, "IV2: ");
    mvprintw(STATE_ROW_9, 0, "MFV: "); //didn't add second display 2nd pin
    mvprintw(STATE_ROW_10, 0, "MOV: ");
    mvprintw(STATE_ROW_11, 0, "IG: ");

    mvprintw(DATA_ROW_0, 0, "DATA HERE");
    mvprintw(DATA_ROW_1, 0, "DATA HERE");
    mvprintw(DATA_ROW_2, 0, "DATA HERE");
    mvprintw(DATA_ROW_3, 0, "DATA HERE");
    mvprintw(DATA_ROW_4, 0, "DATA HERE");
    mvprintw(DATA_ROW_5, 0, "DATA HERE");

    mvprintw(SAVE_FILE_ROW, 0, "Saving to: ");

    mvprintw(LAST_COMMAND_ROW, 0, "Last Command: ");
    mvprintw(INPUT_ROW, 0, "Input: "); 

    return 1;
}

int CommandControl::
char_aval() {
    int cx, cy, input;

    input = getch();

    getyx(stdscr, cy, cx);
    move(cy, cx-1);
    clrtoeol();
    move(cy, cx);

    if(input != ERR) { // no input
        ungetch(input);
        return 1;
    }

    return 0;
}

int CommandControl::
read_input() {

    if(char_aval()) {
        move(INPUT_ROW, 7);
        clrtoeol(); // clear input row

        input_buf[i] = getch();

        if(input_buf[i] == EMERGENCY_KEY) { // Go into an emergency 
            CM->emergency();
            i=0;
            input_buf[i] = '\0';
        }
        else if(input_buf[i] == 10) { // 10 is ascii for enter/return
            // clear output on last command row
            move(LAST_COMMAND_ROW, 14);
            clrtoeol();

            // swap '\n' to '\0' and send to CM
            input_buf[i] = '\0';
            command = input_buf; 
            CM->input_command(command);

            // reset input buffer
            i=0;
            input_buf[i] = '\0';
        }
        else if(input_buf[i] == 8) { // 8 is ascii for backspace
            if(i > 0) { // catch all backspace with on an empty buffer
                --i;
                input_buf[i] = '\0';
            }
        }
        else { // add input chars to buffer
            ++i;
            input_buf[i] = '\0';
            i%=(CMD_BUF_SZ-1); // the last slot in the array is alway reserved for '\0'
        }
    }

    return 1;
}


int CommandControl::
print_state_info(bool & valve_state, int print_row) { 
    move(print_row, STATE_LABEL_LENGTH);
    clrtoeol();
    if(valve_state == OPEN)
        mvprintw(print_row, STATE_LABEL_LENGTH, "%s", "OPEN");
    else
        mvprintw(print_row, STATE_LABEL_LENGTH, "%s", "CLOSED");
    return 1;
}

int CommandControl::
print_data() {
    move(STATE_ROW_0, 15);
    clrtoeol();

    if(state.in_emergency) {
        attron(COLOR_PAIR(1)); // Color on
        mvprintw(STATE_ROW_0, col/2, "EMERGENCY");
        attroff(COLOR_PAIR(1)); // Color off
    }

    attron(COLOR_PAIR(2)); // Color on

    mvprintw(STATE_ROW_0, 15, "%s", state.current_state_name.c_str());

    print_state_info(state.APC, STATE_ROW_1);
    print_state_info(state.VVO, STATE_ROW_2); 
    print_state_info(state.VVF, STATE_ROW_3); 
    print_state_info(state.OPV, STATE_ROW_4); 
    print_state_info(state.FPV, STATE_ROW_5); 
    print_state_info(state.PPV, STATE_ROW_6); 
    print_state_info(state.IV1, STATE_ROW_7); 
    print_state_info(state.IV2, STATE_ROW_8); 

    move(STATE_ROW_10, STATE_LABEL_LENGTH);
    clrtoeol();

    if(state.MFV == OPEN)
        mvprintw(STATE_ROW_10, STATE_LABEL_LENGTH, "%s", "OPEN");
    else if(state.MFV == CRACKED)
        mvprintw(STATE_ROW_10, STATE_LABEL_LENGTH, "%s", "CRACKED");
    else if(state.MFV == CLOSED)
        mvprintw(STATE_ROW_10, STATE_LABEL_LENGTH, "%s", "CLOSED");

    if(state.MOV == OPEN)
        mvprintw(STATE_ROW_10, STATE_LABEL_LENGTH, "%s", "OPEN");
    else if(state.MOV == CRACKED)
        mvprintw(STATE_ROW_10, STATE_LABEL_LENGTH, "%s", "CRACKED");
    else if(state.MOV == CLOSED)
        mvprintw(STATE_ROW_10, STATE_LABEL_LENGTH, "%s", "CLOSED");

    move(STATE_ROW_11, STATE_LABEL_LENGTH);
    clrtoeol();
    if(state.IG == ON)
        mvprintw(STATE_ROW_11, STATE_LABEL_LENGTH, "%s", "ON");
    else
        mvprintw(STATE_ROW_11, STATE_LABEL_LENGTH, "%s", "OFF");

    move(SAVE_FILE_ROW, 10);
    clrtoeol();
    mvprintw(SAVE_FILE_ROW, 10, "%s", state.save_file_name.c_str());
    if(state.saving == 1)
        mvprintw(SAVE_FILE_ROW, 40, "%s", "Saving");
    else if(state.saving == 0)
        mvprintw(SAVE_FILE_ROW, 40, "%s", "NOT Saving");
    else
        mvprintw(SAVE_FILE_ROW, 40, "%s", "ERROR, NOT Saving");

    mvprintw(LAST_COMMAND_ROW, 14, "%s", command.c_str());
    mvprintw(INPUT_ROW, 7, "%s", input_buf);

    attroff(COLOR_PAIR(2)); // Color off

    return 1;
}

