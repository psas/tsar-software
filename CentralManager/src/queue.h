#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <cstring>
#include <mutex>

class queue {
    public:
        queue();

        int enqueue(const send_data & input);
        int dequeue(struct send_data & output);
        int status();
    private:
        send_data data[SEND_DATA_Q_LEN];
        int queue_length;
        int q_data_size;
        int current_count;
        int current_enqueue;
        int current_dequeue;
        int send_data_size;
        std::mutex queue_mutex;
};

#endif
