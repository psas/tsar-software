#include "queue.h"

template <typecast T>


// main constructor
queue::
queue(T data_type, int length) : queue_length(SEND_DATA_Q_LEN), current_count(0), 
        current_enqueue(0), current_dequeue(0) {
    send_data_size = sizeof(data_type);
    data = new T[length];
}


/* If the dequeue ref and enqueue ref are equal it will move the dequeue ref,
 * or in other words it overrides the dequeue data if the queue is full.
 */
int queue::
enqueue(T & input) {
    queue_mutex.lock();

    // copy input
    memcpy(&data[current_enqueue], &input, send_data_size);

    ++current_count;

    ++current_enqueue %= queue_length;
    if(current_enqueue == current_dequeue) { // queue is full
        ++current_dequeue %= queue_length;
        queue_mutex.unlock();
        return 0;
    }

    queue_mutex.unlock();
    return 1;
}


/* This will dequeue the queue data into the area of the output pointer 
 * and then moving the dequeue reference for the next dequeue.
 */
int queue::
dequeue(T & output) {
    queue_mutex.lock();
    if(current_dequeue == current_enqueue) {// queue is empty
        queue_mutex.unlock();
        return 0;
    }

    // override ouput
    memcpy(&output, &data[current_dequeue], send_data_size);

    --current_count;

    ++current_dequeue %= queue_length;
    queue_mutex.unlock();
    return 1;
}


// get current count of items in the queue
int queue::
status() { 
    int rc = -1;
    queue_mutex.lock();
    rc = current_count; 
    queue_mutex.unlock();
    return rc; 
}
