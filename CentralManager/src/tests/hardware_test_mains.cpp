#include <iostream>

/*
 * This file contatins all hardware related test main()
 */

#include <thread>
#include <memory>
#include <iostream>

#include "../hardware_controller.h"
#include "../link_logger.h"

using namespace std;

int
main() {
    cout << "Starting Hardware with Link Test" << endl;

    shared_ptr<link_logger> link;

#ifdef LL_OFF
    shared_ptr<server> serv = server();
    link = link_logger(serv);
    std::thread serv_thread(&server::driver_loop, ref(serv));
    std::thread ll_thread(&link_logger::driver_loop, ref(link));
#endif // LL_OFF

    hardware_controller hdw(link);

    hdw.driver_loop();

    return 0;
}

