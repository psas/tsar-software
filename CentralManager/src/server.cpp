#include "server.h"

// defualt constuctor
server::
server() : send_q(SEND_Q_LENGTH, std::string(SEND_INIT_SIZE, '\0')),  
        recv_q(RECV_Q_LENGTH, std::string(RECV_BUFF_SIZE,'\0')), 
        driver_running(0) {

    // clear the fds_master_list and temp sets
    FD_ZERO(&fds_master_list);

    // listener
    if((listener = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        throw ServerException("Socket Error");

    // listener setsockopt
    int option_value = 1;
    if(setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &option_value, sizeof(int)) == -1)
        throw ServerException("SetSocketOpt Error");

    address_len = sizeof(struct sockaddr_in);

    // listener bind
    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = INADDR_ANY;
    serveraddr.sin_port = htons(PORT);
    memset(&(serveraddr.sin_zero), '\0', 8); // to avoid undefined behavior in padding bytes
    if(bind(listener, (struct sockaddr *)&serveraddr, sizeof(struct sockaddr_in)) == -1)
        throw ServerException("Bind Error");

    // listen
    if(listen(listener, 10) == -1)
        throw ServerException("Listen Error");

    FD_SET(listener, &fds_master_list); // add listener to master list
    fd_count = listener; // keep track of the largest descriptor
    
    //delays
    pselect_timeout.tv_sec = 0;
    pselect_timeout.tv_nsec = PSELECT_TIMEOUT * 1000000;
    driver_delay.tv_sec = 0;
    driver_delay.tv_nsec = SERVER_DELAY * 1000000;    

    driver_running = 0;
}


/* Main server driver loop function. Alway checking for new client, 
 * recieving new message from client(s), and sending queue data to client(s).
 */
void server::
driver_loop() {
    std::string message;

    std::cout << "server connection opened\n";

    driver_running = 1;
    while(driver_running == 1) { 
        try {
        check_new_and_read(); //this will also handle enqueuing into recv_q
        while(send_q.dequeue(message) == 1) // has data to send
            send_to_all(message);
        }
        catch (ServerException & e){
            std::cout << e.what() << std::endl;
            break; //TODO: change this
        }
        nanosleep(&driver_delay, NULL);
    }

    std::cout << "server connection closed\n";
    return;
}


/* this will read any mnd all message from client(s) 
 * if it doesn't recognize the client it will add it to master list of clients
 */
int server::
check_new_and_read() {
    fd_set fds_read_list = fds_master_list; // copy it
    std::string buffer(RECV_BUFF_SIZE,'\0');
    struct sockaddr_in clientaddr; // address for new client
    int new_fd;

    // this will search this list for new data received until pselect_timeout, 0 is no data
    int is_data = pselect(fd_count+1, &fds_read_list, NULL, NULL, 
            &pselect_timeout, NULL);
    if(is_data == 0) // pselect_timeout, no data
        return 0;
    else if(is_data == -1)
        throw  ServerException("Pselect Error");

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
                else
                    throw ServerException("Accept Error");
                    // TODO fd_zero, all fd_data is assume to be bad
            }
            else { // handle data from a client
                if(recv(i, &buffer[0], RECV_BUFF_SIZE, 0) <= 0) {
                    // got error (1) or connection closed by client (0)
                    std::cout << "Socket " << i << " disconnected/errored\n";
                    close(i);
                    FD_CLR(i, &fds_master_list); // remove from master set
                }
                else{ // got data from a client, add it to queue
                    std::cout << "Command Accepted\n";
                    buffer[RECV_BUFF_SIZE-1] = '\0'; // just incase
                    recv_q.enqueue(buffer);
                }
            }
        }
    }
    return 1;
}


// sends input string to all clients
int server::
send_to_all(const std::string & message) {
    for(int i=0; i <= fd_count; i++) { // send to each client
        if(FD_ISSET(i, &fds_master_list) && (i != listener)) {// don't send to listener and itself (the server)
            if(send(i, &message[0], strlen(&message[0])+1, 0) <= 0) { 
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
kill_driver() { 
    serv_mutex.lock();
    driver_running = 0; 
    serv_mutex.unlock();
}


// enqueue message to be send
int server::
send_string(const std::string & message) {
    int ri = 0;
    serv_mutex.lock();
    ri = send_q.enqueue(message); 
    serv_mutex.unlock();
    return ri;
}


// dequeue oldest recieve message
int server::
recv_string(std::string & message) {
    int ri = 0;
    serv_mutex.lock();
    ri = recv_q.dequeue(message);
    serv_mutex.unlock();
    return ri;
}
