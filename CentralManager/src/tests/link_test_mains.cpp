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
    std::string temp = {"THIS IS A TEST"};
    
    try {
        server serv;
        std::thread serv_thread(&server::driver_loop, std::ref(serv));

        while(1) {
            serv.send_string(temp);
            sleep(1);
        }
    } catch(ServerException& e){
        std::cout << e.what() << std::endl;
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

