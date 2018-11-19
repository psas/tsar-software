#ifndef _STRING_QUEUES_H_
#define _STRING_QUEUES_H_

#include <cstring>
#include <iostream>
#include <mutex>

class string_queue {
    public:
        string_queue();
        string_queue(const int & queue_length, const int & string_length);
        ~string_queue();

        int enqueue(const char * input);
        int dequeue(char * output);
        int status();
    private:
        char ** data;
        int string_length;
        int queue_length;       // protected by mutex
        int pointer_size;       // protected by mutex
        int current_count;      // protected by mutex
        int current_enqueue;    // protected by mutex
        int current_dequeue;    // protected by mutex
        std::mutex queue_mutex;
};

#endif
