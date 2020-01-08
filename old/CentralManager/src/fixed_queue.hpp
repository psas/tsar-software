#ifndef _FIXED_QUEUE_H_
#define _FIXED_QUEUE_H_

#include <mutex>
#include <vector>


/* Why this class is needed:
 * The Link Logger and Server both need queues to act as buffer for data to be sent and
 * data recieved. Since it is possible for the Sequencer and Hardware Controller to send
 * out their status/data faster than the Link Logger (and Server) can process that data,
 * a fixed queue is needed for the send queues. A std::queue would could endless grow 
 * in a worse case scenario.
 *
 * How this Class works:
 * This class implements a fixed length queue by a using an vector as a circular array.
 * The vector length is set during the constructor and can never change after. 
 * If the queue is full, the oldest enqueue data will be overriden when a new enqueue 
 * occurs. 
 */
template <typename T>
class fixed_queue {
    public:
        // default constructor
        fixed_queue(const unsigned int in_q_len);

        // fill constructor
        fixed_queue(const unsigned int in_q_len, const T & input);

        int enqueue(const T & input);
        int dequeue(T & output);

        // return number of items in queue currently
        int size();
    private:
        std::vector<T> data;
        unsigned int length;                // size of container (the vector length)
        unsigned int current_count;         // count of data enqueued
        unsigned int current_enqueue;       // current enqueue posistion in vector
        unsigned int current_dequeue;       // current dequeue posistion in vector
        std::mutex queue_mutex;
};


// constructor, with vector default constructor
template <typename T>
fixed_queue<T>::
fixed_queue(const unsigned int in_q_len) : 
        data(in_q_len+1), length(in_q_len), current_count(0), 
        current_enqueue(0), current_dequeue(0) {}


// constructor, with vector fill constructor
template <typename T>
fixed_queue<T>::
fixed_queue(const unsigned int in_q_len, const T & input) : 
        data(in_q_len+1, input), length(in_q_len), current_count(0), 
        current_enqueue(0), current_dequeue(0) {}


// enqueue data, will not fail
template <typename T>
int fixed_queue<T>::
enqueue(const T & input) {
    queue_mutex.lock();

    data[current_enqueue] = input;

    ++current_enqueue %= length;
    if(current_enqueue == current_dequeue) { // queue is full
        ++current_dequeue %= length;         // move dequeue flag over one spot
        queue_mutex.unlock();
        return 2;
    }
    else
        ++current_count;
    
    queue_mutex.unlock();
    return 1;
}


// dequeue data
// if empty return 0, otherwise return 1
template <typename T>
int fixed_queue<T>::
dequeue(T & output) {
    queue_mutex.lock();

    if(current_dequeue == current_enqueue) { // queue is empty
        queue_mutex.unlock();
        return 0;
    }

    output = data[current_dequeue];
    --current_count;

    ++current_dequeue %= length;
    queue_mutex.unlock();
    return 1;
}


// get current count of items in the queue
template <typename T>
int fixed_queue<T>::
size() {
    int rc = -1;
    queue_mutex.lock();
    rc = current_count;
    queue_mutex.unlock();
    return rc;
}

#endif // _FIXED_QUEUE_
