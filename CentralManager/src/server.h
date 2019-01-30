#ifndef _SERVER_H_
#define _SERVER_H_

#include <sys/types.h>              // send, recv
#include <sys/socket.h>             // socket, setsocketopt, bind, listen, accept
#include <sys/select.h>             // pselect, fd
#include <netinet/in.h>             // address_in
#include <arpa/inet.h>              // inet_ntoa 
#include <unistd.h>                 // close
#include <iostream>                 // cout, cerr
#include <cstring>                  // memset
#include <vector>                   // vector
#include <string>                   // exeception messages
#include <mutex>                    // mutex

#include "fixed_queue.h"

#define PORT 8080
#define PSELECT_TIMEOUT 5           // milliseconds
#define SERVER_DELAY 2              // milliseconds
#define RECV_VECTOR_BUFF_SIZE 100
#define SEND_VECTOR_INIT_SIZE 200
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


/* server:
 */
class server {
    public:
        server();

        void driver_loop();
        int send_string(const std::vector<char> & message);
        int recv_string(std::vector<char> & message);
        void kill_driver();
    private:
        int check_new_and_read();
        int send_to_all(const std::vector<char> & message);

        // -------- data ---------
        fixed_queue<std::vector<char>> send_q;
        fixed_queue<std::vector<char>> recv_q;

        // file descriptor lists
        fd_set fds_master_list;

        int fd_count; // maximum file descriptor number
        int listener; // listening socket descriptor

        struct timespec pselect_timeout;
        struct timespec driver_delay;

        int address_len;

        int driver_running;
        std::mutex serv_mutex;
};

#endif
