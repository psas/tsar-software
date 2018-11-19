#ifndef _SERVER_H_
#define _SERVER_H_

#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <cstring>

#include "string_queue.h"

#define PORT 8080
#define PSELECT_TIMEOUT 5 // milliseconds
#define SERVER_DELAY 20 // milliseconds
#define SEND_STRING_MAX_LEN 2048
#define RECV_STRING_MAX_LEN 128
#define SEND_STRING_Q_MAX_LEN 50
#define RECV_STRING_Q_MAX_LEN 50

/* server:
 */
class server {
    public:
        server();

        void driver_loop();
        int send_string(const char * message);
        int recv_string(char * message);
        void kill_driver();
#ifdef SERVER_TEST
        void test_driver_loop();
#endif
    private:
        int check_new_and_read();
        int send_to_all(const char * send_temp);

        // -------- data ---------
        string_queue send_q;
        string_queue recv_q;

        // file descriptor lists
        fd_set fds_master_list;


        int fd_count; // maximum file descriptor number
        int listener; // listening socket descriptor

        struct timespec pselect_timeout;
        struct timespec driver_delay;

        int address_len;

        int driver_running;
};

#endif
