#include <iostream>

/*
 * This file contatins all hardware related test main()
 */

#include <thread>
#include <memory>
#include <iostream>
#include "../hardware_controller.h"


int
main() {
    std::cout << "Starting Hardware with Link Test" << std::endl;

    std::shared_ptr<link_logger> link;

#ifdef HARDWARE_LINK_TEST
    //TODO start threads as needed
    std::shared_ptr<server> serv;
    link = link_logger(serv)
#endif // HARDWARE_LINK_TEST

    hardware_controller hdw(link);

    hdw.driver_loop();

    return 0;
}

