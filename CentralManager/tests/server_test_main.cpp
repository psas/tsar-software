#include <iostream>
#include <string>
#include <chrono>
#include <thread>

#include "../server.h"

using namespace std;

int
main() {
    std::cout << "Starting Server Test" << std::endl;
    std::string temp = {"THIS IS A TEST"};
    std::string temp2;
    
    try {
        server serv;
        std::thread serv_thread(&server::driver_loop, std::ref(serv));

        while(1) {
            serv.send_string(temp);
            if(serv.recv_string(temp2) ) {
                cout << temp2 << endl;
            }
            this_thread::sleep_for(chrono::microseconds(500));
        }

        if(serv_thread.joinable())
            serv_thread.join();

    }
    catch(CM_Exception& e){
        cout << e.what() << endl;
    }
    catch(...) {
        cout << "Unknown Expection" << endl;
    }

    return 0;
}
