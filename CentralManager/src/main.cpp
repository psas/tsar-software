#include <iostreamm>

#if defined(SERVER_TEST)
#include "server.h"
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
#elif defined(HARDWARE_TEST)
#include "hardware_controller.h"
int
main() {
    hardware_controller hdw;
    hdw.driver_loop();
    return 0;
}
#elif defined(HARDWARE_LINK_TEST)
#include <thread>
#include "hardware_controller.h"
int
main() {
    link_logger link;
    hardware_controller hdw(&link);

    std::thread * link_thread;
    link_thread = new std::thread(&link_logger::driver_loop, &link);

    hdw.driver_loop();
    return 0;
}
#elif defined(LINK_TEST)
#include "link_logger.h"
int
main() {
    link_logger link;
    link.driver_loop();
    return 0;
}
#else
#include "central_manager.h"
int
main() {
    central_manager CM;
    CM.start_system();
    return 0;
}
#endif

