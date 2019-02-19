#ifndef _FIXED_QUEUE_H_
#define _FIXED_QUEUE_H_

#include <mutex>
#include <vector>

/*
 * This class implament ad fixed length queue by a using an array to 
 */
template <typename T>
class fixed_queue {
    public:
        fixed_queue(const unsigned int in_q_len);
        fixed_queue(const unsigned int in_q_len, const T & input);

        int enqueue(const T & input);
        int dequeue(T & output);
        int status();
    private:
        std::vector<T> data;
        unsigned int length;
        unsigned int current_count;
        unsigned int current_enqueue;
        unsigned int current_dequeue;
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
status() {
    int rc = -1;
    queue_mutex.lock();
    rc = current_count;
    queue_mutex.unlock();
    return rc;
}

#endif // _FIXED_QUEUE_
