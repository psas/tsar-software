#include <iostream>

/*
 * This file contatins all hardware related test main()
 */

#if defined(HARDWARE_TEST)

#include "../hardware_controller.h"

int
main() {
    std::cout << "Starting Hardware Test" << std::endl;

    hardware_controller hdw;
    hdw.driver_loop();

    return 0;
}


#elif defined(HARDWARE_LINK_TEST)

#include <thread>
#include "../hardware_controller.h"

int
main() {
    std::cout << "Starting Hardware with Link Test" << std::endl;

    link_logger link;
    hardware_controller hdw(&link);

    std::thread * link_thread;
    link_thread = new std::thread(&link_logger::driver_loop, &link);

    hdw.driver_loop();
    return 0;
}

#endif

