#include "string_queue.h"


// default constructor
string_queue::
string_queue() : data(NULL), string_length(0), queue_length(0),
        current_count(0), current_enqueue(0), current_dequeue(0) {}


// main constructor, choose the length of queue and string length
string_queue::
string_queue(const int & in_q_len, const int & in_string_len) : 
        string_length(in_string_len), queue_length(in_q_len), current_count(0), 
        current_enqueue(0), current_dequeue(0) { 
    data = new char *[in_q_len];
    for(int i=0; i<in_q_len; ++i)
        data[i] = new char[in_string_len];
}


// deconstructor
string_queue::
~string_queue() {
    if(data != NULL) {
        for(int i=0; i<queue_length; ++i) {
            if(data[i] != NULL) {
                delete [] data[i];
                data[i] = NULL;
            }
        }
        delete [] data;
        data = NULL;
    }
}


/* This will enqueue the string by copying the data to the enqueue ref location 
 * and then moving the enqueue reference.
 * If the dequeue ref and enqueue ref are equal it will move the dequeue ref,
 * or in other words it overrides the dequeue data if the queue is full.
 */
int string_queue::
enqueue(const char * input) {
    queue_mutex.lock();

    if(data == NULL){ // queue is not initiated
        std::cout << "queue is not initiated" << std::endl;
        queue_mutex.unlock();
        return -1;
    }

    // turncate string if too long
    int len = strlen(input)+1;
    if(len > string_length)
        len = string_length;


    // copy input
    strncpy(data[current_enqueue], input, len);

    ++current_count;

    ++current_enqueue %= queue_length;
    if(current_enqueue == current_dequeue) { // queue is full
        ++current_dequeue %= queue_length;
        std::cout << "QUEUE IS FULL\n";
        queue_mutex.unlock();
        return 0;
    }
    
    queue_mutex.unlock();
    return 1;
}


/* This will dequeue the queue data into the area of the output pointer 
 * and then moving the dequeue reference for the next dequeue.
 */
int string_queue::
dequeue(char * output) {
    queue_mutex.lock();

    if(data == NULL){ // queue is not initiated
        queue_mutex.unlock();
        std::cout << "queue is not initiated" << std::endl;
        return -1;
    }

    if(current_dequeue == current_enqueue) { // queue is empty
        queue_mutex.unlock();
        return 0;
    }

    // override ouput
    strncpy(output, data[current_dequeue], strlen(data[current_dequeue])+1);

    --current_count;

    ++current_dequeue %= queue_length;
    queue_mutex.unlock();
    return 1;
}


// get current count of items in the queue
int string_queue::
status() {
    int rc = -1;
    queue_mutex.lock();
    rc = current_count;
    queue_mutex.unlock();
    return rc;
}
