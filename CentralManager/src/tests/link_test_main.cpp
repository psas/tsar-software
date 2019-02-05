#include <iostream>

/*
 * This file contatins all link logger and server related test main()
 */

#if defined(SERVER_TEST)

#include "../server.h"
#include <string>
#include <ctime>
#include <thread>

int
main() {
    try {
    server serv;
    } catch(ServerException& e){
        std::cout << e.what() << std::endl;
    }

    std::thread * serv_thread;
    serv_thread = new std::thread(&server::driver_loop, &serv);

    std::string temp = {"THIS IS A TEST"};

    while(1) {
        serv.send_string(temp);
        sleep(1);
    }

    return 0;
}

#elif defined(LINK_TEST)

#include "../link_logger.h"

int
main() {
    link_logger link;
    link.driver_loop();
    return 0;
}

#endif

