#include <iostream>
#include <thread>
#include <memory>
#include <iostream>

#include "../hardware/hardware_controller.h"
#include "../link_logger.h"
#include "../server.h"

using namespace std;


#ifndef LL_OFF
void
run_server(shared_ptr<server> & s) {
    std::cout << "Server thread has started." << std::endl;
    pthread_setname_np(pthread_self(), "Server");
    s->driver_loop();
    std::cout << "Server thread has stoped." << std::endl;
}
void
run_link(shared_ptr<link_logger> & l) {
    std::cout << "Link Logger thread has started." << std::endl;
    pthread_setname_np(pthread_self(), "LinkLogger");
    l->driver_loop();
    std::cout << "LinkLogger thread has stoped." << std::endl;
}
#endif // LL_OFF

int
main() {
    cout << "Starting Hardware with Link Test" << endl;

    shared_ptr<link_logger> link;

#ifndef LL_OFF
    shared_ptr<server> serv = make_shared<server>();
    link = make_shared<link_logger>(serv);
    cout << "Server thread starting" << endl;
    std::thread server_thread(run_server, ref(serv));
    cout << "Link Logger thread starting" << endl;
    std::thread link_thread(run_link, ref(link));
#endif // LL_OFF

    hardware_controller hdw(link);

    hdw.driver_loop();

#ifndef LL_OFF
    link->stop_driver();
    serv->stop_driver();

    if(link_thread.joinable())
        link_thread.join();
    if(server_thread.joinable())
        server_thread.join();
#endif // LL_OFF

    return 0;
}

