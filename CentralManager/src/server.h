#ifndef _SERVER_H_
#define _SERVER_H_

#include <sys/types.h>              // send, recv
#include <sys/socket.h>             // socket, setsocketopt, bind, listen, accept
#include <sys/select.h>             // pselect, fd
#include <netinet/in.h>             // address_in
#include <arpa/inet.h>              // inet_ntoa 
#include <unistd.h>                 // close
#include <cstring>                  // memset
#include <string>                   // exeception messages
#include <mutex>                    // mutex
#include <chrono>                   // mircoseconds
#include <ctime>                    // timespec
#include <thread>                   // sleep_for
#include <atomic>                   // atomic
#include <iostream>                 // cout, cerr

#include "fixed_queue.h"

#define PORT 8080
#define PSELECT_TIMEOUT 5           // milliseconds
#define SERVER_DELAY 50             // milliseconds
#define RECV_BUFF_SIZE 100
#define SEND_INIT_SIZE 200
#define SEND_Q_LENGTH 100
#define RECV_Q_LENGTH 50

// for throwing exceptions in the constructor of server class
class ServerException {
    private:
        std::string msg;
    public: 
        ServerException(std::string str) : msg(str) {}
        std::string what() const {return msg;}
};


// Asynchronous TCP server.
class server {
    public:
        server();

        void driver_loop();
        int send_string(const std::string & message);
        int recv_string(std::string & message);
        void stop_driver();
    private:
        int check_new_and_read();
        int send_to_all(const std::string & message);

        // buffers
        fixed_queue<std::string> send_q;
        fixed_queue<std::string> recv_q;

        // file descriptor lists
        fd_set fds_master_list;

        int fd_count; // maximum file descriptor number
        int listener; // listening socket descriptor

        struct timespec pselect_timeout;

        int address_len;

        std::atomic<bool> driver_running;
        std::mutex serv_mutex;
};

#endif
