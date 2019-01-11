#include "server.h"


// defualt constuctor
server::
server() : send_q(SEND_STRING_Q_MAX_LEN, SEND_STRING_MAX_LEN), 
    recv_q(RECV_STRING_Q_MAX_LEN, RECV_STRING_MAX_LEN), driver_running(0) {

    // clear the fds_master_list and temp sets
    FD_ZERO(&fds_master_list);

    // listener
    if((listener = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        std::cout << "Socket Error\n";

    // listener setsockopt
    int yes = 1; // don't why it takes a pointer to 1
    if(setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
        std::cout << "SetScoketOpt Error\n";

    address_len = sizeof(struct sockaddr_in);

    // listener bind
    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = INADDR_ANY;
    serveraddr.sin_port = htons(PORT);
    memset(&(serveraddr.sin_zero), '\0', 8);
    if(bind(listener, (struct sockaddr *)&serveraddr, 
                sizeof(struct sockaddr_in)) == -1)
        std::cout << "Bind Error\n";

    // listen
    if(listen(listener, 10) == -1)
        std::cout << "Listen Error\n";

    FD_SET(listener, &fds_master_list); // add listener to master list
    fd_count = listener; // keep track of the largest descriptor
    
    //delays
    pselect_timeout.tv_sec = 0;
    pselect_timeout.tv_nsec = PSELECT_TIMEOUT * 1000000;
    driver_delay.tv_sec = 0;
    driver_delay.tv_nsec = SERVER_DELAY * 1000000;    
}


/* Main server driver loop function. Alway checking for new client, 
 * recieving new message from client(s), and sending queue data to client(s).
 */
void server::
driver_loop() {
    char message[SEND_STRING_MAX_LEN];

    std::cout << "Server connection open\n";

    driver_running = 1;
    while(driver_running == 1) { 
        check_new_and_read(); //this will also handle enqueuing into recv_q

        while(send_q.dequeue(message) == 1) // has data to send
            send_to_all(message);
        nanosleep(&driver_delay, NULL);
    }

    std::cout << "Server connection closed\n";
    return;
}


#ifdef SERVER_TEST
void server::
test_driver_loop() {
    char send_message[SEND_STRING_MAX_LEN] = "THIS IS A TEST\n";
    char recv_message[RECV_STRING_MAX_LEN];

    std::cout << "Server connection open\n";
    
    while(1) { 
        if(check_new_and_read()) {
            std::cout << "Message recv: " << recv_message << std::endl;
            recv_string(recv_message); // so recv queue doesnt become full
        }
        send_to_all(send_message);
        nanosleep(&driver_delay, NULL);
    }
    
    std::cout << "Server connection closed\n";
    
    return;
}
#endif


/* this will read any mnd all message from client(s) 
 * if it doesn't recognize the client it will add it to master list of clients
 */
int server::
check_new_and_read() {
    fd_set fds_read_list = fds_master_list; // copy it

    // this will search this list for new data received until pselect_timeout, 0 is no data
    int is_data = pselect(fd_count+1, &fds_read_list, NULL, NULL, 
            &pselect_timeout, NULL);
    if(is_data == -1) {
        std::cout << "Select Error -- pselect\n";
        return -1;
        // remove all clients??? <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    }
    else if(is_data == 0) // pselect_timeout, no data
        return 0;

    char buf[RECV_STRING_MAX_LEN];
    struct sockaddr_in clientaddr; // address for new client
    int new_fd;

    // checking all existing connections looking for data to be received
    // also will handle any new client
    for(int i=0; i <= fd_count; i++) {
        if(FD_ISSET(i, &fds_read_list)) { // check if is i is in fd_list
            if(i == listener) { // handle new connections 
                new_fd = accept(listener, (struct sockaddr *)&clientaddr,
                        (socklen_t *)&address_len);
                if(new_fd != -1){
                    FD_SET(new_fd, &fds_master_list); // add to fds_master_list set
                    if(new_fd > fd_count) // keep track of the maximum
                        fd_count = new_fd;
                    std::cout << "New connection from " 
                         << inet_ntoa(clientaddr.sin_addr) << " on socket "
                         << new_fd << std::endl;
                }
                else {
                    std::cout << "Accept Error -- Receiver\n";
                    return 0;
                }
            }
            else { // handle data from a client
                // testing neeed on this, how does it handle no data? <<<<<<<<<
                if(recv(i, buf, RECV_STRING_MAX_LEN, 0) <= 0) {
                    // got error (1) or connection closed by client (0)
                    std::cout << "Socket " << i << " disconnected/errored\n";
                    close(i);
                    FD_CLR(i, &fds_master_list); // remove from master set
                }
                else{ // got data from a client, add it to queue
                    std::cout << "Command Accepted\n";
                    // json to struct convert
                    buf[RECV_STRING_Q_MAX_LEN-1] = '\0'; // just incase
                    recv_q.enqueue(buf);
                }
            }
        }
    }
    return 1;
}


// sends input string to all clients
int server::
send_to_all(const char * message) {
    int len = strlen(message);
    if(len == 0)
        return 0;

    // check intial i value <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    for(int i=0; i <= fd_count; i++) { // send to each client
        if(FD_ISSET(i, &fds_master_list) && (i != listener)) {
            if(send(i, message, len+1, 0) <= 0) { 
                // error or connection closed
                std::cout << "Socket " << i << " disconnected/errored\n";
                close(i);
                FD_CLR(i, &fds_master_list); // remove from master list
            }
        }
    }
    return 1;
}


//stops driver_loop, only use to end server
void server::
kill_driver() { driver_running = 0; }


// enqueue message to be send
int server::
send_string(const char * message) {
    if(strlen(message) == 0)
        return 0;
    return send_q.enqueue(message); 
}


// dequeue oldest recieve message
int server::
recv_string(char * message) { 
    return recv_q.dequeue(message);
}
